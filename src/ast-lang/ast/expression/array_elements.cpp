#include <ast-lang/ast/expression/array_elements.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    using namespace interpreter;

    auto ArrayElements::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        const auto front_node = ConstNodePtr{front().get()};
        auto result = std::vector<EvaluationResult>{};
        auto first_element = front_node.astlangNode->compute(interpreter);
        auto first_element_type = first_element.type;

        result.emplace_back(std::move(first_element));

        if (size() > 1) {
            const auto back_node = ConstNodePtr{back().get()};
            auto other_elements = back_node.astlangNode->compute(interpreter);
            auto unpacked_elements = other_elements.value.release<std::vector<EvaluationResult>>();

            for (auto &element : *unpacked_elements) {
                if (element.type != first_element_type) {
                    first_element_type = Type::ANY;
                }

                result.emplace_back(std::move(element));
            }
        }

        return EvaluationResult{
            .value = isl::UniqueAny{std::move(result)},
            .type =
                Type{
                    Type::LIST,
                    {
                        first_element_type,
                    },
                },
        };
    }
}// namespace astlang::ast::expression
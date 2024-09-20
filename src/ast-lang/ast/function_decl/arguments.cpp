#include <ast-lang/ast/function_decl/arguments.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::function::decl
{
    using namespace astlang::interpreter;

    auto Arguments::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        auto result = std::vector<EvaluationResult>{};
        const auto front_node = ConstNodePtr{front().get()};

        if (size() == 1) {
            result.emplace_back(front_node.astlangNode->compute(interpreter));

            return EvaluationResult{
                isl::UniqueAny(std::move(result)),
                Type{Type::LIST, {Type::ANY}},
            };
        }

        const auto back_node = ConstNodePtr{back().get()};
        auto next_arguments = back_node.astlangNode->compute(interpreter);
        auto resulted_list = next_arguments.value.release<std::vector<EvaluationResult>>();

        for (auto &argument : *resulted_list) {
            result.emplace_back(std::move(argument));
        }

        return EvaluationResult{
            isl::UniqueAny(std::move(result)),
            Type{Type::LIST, {Type::ANY}},
        };
    }
}// namespace astlang::ast::function::decl
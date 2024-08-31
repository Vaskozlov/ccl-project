#include <ast-lang/ast/function_call/arguments.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::function::call
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

        auto *casted_next_arguments = isl::observe<EvaluationResult>(next_arguments.value);

        auto *resulted_list =
            isl::observe<std::vector<EvaluationResult>>(casted_next_arguments->value);

        for (auto &argument : *resulted_list) {
            result.emplace_back(std::move(argument));
        }

        return EvaluationResult{
            isl::UniqueAny(std::move(result)),
            Type{Type::LIST, {Type::ANY}},
        };
    }
}// namespace astlang::ast::function::call
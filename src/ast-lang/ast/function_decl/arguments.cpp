#include <ast-lang/ast/function_decl/arguments.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::function::decl
{
    using namespace astlang::interpreter;

    auto Arguments::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        // TODO
        auto result = std::vector<EvaluationResult>{};
        auto front_node = ConstNodePtr{this->front()};

        if (this->size() == 1) {
            result.emplace_back(front_node.astlangNode->compute(interpreter));

            return EvaluationResult{
                isl::UniqueAny(std::move(result)),
                Type{Type::LIST, {Type::ANY}},
            };
        }

        auto back_node = ConstNodePtr{this->back()};
        auto next_arguments = back_node.astlangNode->compute(interpreter);
        auto resulted_list = isl::get<std::vector<EvaluationResult>>(next_arguments.value);

        for (auto &argument : resulted_list) {
            result.emplace_back(std::move(argument));
        }

        return EvaluationResult{
            isl::UniqueAny(std::move(result)),
            Type{Type::LIST, {Type::ANY}},
        };
    }
}// namespace astlang::ast::function::decl
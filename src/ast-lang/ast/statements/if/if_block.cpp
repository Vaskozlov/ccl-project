#include <ast-lang/ast/statements/if/if_block.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::statement::if_statement
{
    auto IfBlock::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        const auto condition = ConstNodePtr{nodes.at(1).get()};
        const auto block = ConstNodePtr{nodes.at(3).get()};

        // if (auto condition_result = condition.astlangNode->compute(interpreter);
        // isl::get<bool>(condition_result.value)) {
        // return block.astlangNode->compute(interpreter);
        // }

        return EvaluationResult{
            // .value = std::nullopt,
            // .type = interpreter::Type::ERROR,
        };
    }
}// namespace astlang::ast::statement::if_statement

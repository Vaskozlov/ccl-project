#include <ast-lang/ast/statements/if/if_block.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::statement::if_statement
{
    auto IfBlock::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        auto condition = ConstNodePtr{nodes.at(1).get()};
        auto block = ConstNodePtr{nodes.at(3).get()};

        auto conditionResult = condition.astlangNode->compute(interpreter);

        if (isl::get<bool>(conditionResult.value)) {
            return block.astlangNode->compute(interpreter);
        }

        return EvaluationResult{.value = std::nullopt, .type = interpreter::Type::ERROR};
    }
}// namespace astlang::ast::statement::if_statement

#include <ast-lang/ast/statements/if/elif_block.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::statement::if_statement
{
    auto ElifBlock::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        auto condition = ConstNodePtr{nodes.at(1)};
        auto block = ConstNodePtr{nodes.at(3)};

        auto conditionResult = condition.astlangNode->compute(interpreter);

        if (isl::get<bool>(conditionResult.value)) {
            return block.astlangNode->compute(interpreter);
        }

        if (size() == 6) {
            auto elif_block = ConstNodePtr{nodes.at(5)};
            return elif_block.astlangNode->compute(interpreter);
        }

        return EvaluationResult{.value = std::nullopt, .type = interpreter::Type::ERROR};
    }
}// namespace astlang::ast::statement::if_statement

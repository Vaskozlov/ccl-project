#include <ast-lang-2/ast/statement/if_block.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::statement
{
    auto IfBlock::compute(interpreter::Interpreter &interpreter) const -> core::ComputationResult
    {
        const auto *condition_node = static_cast<const AstlangNode *>(nodes.at(1).get());
        const auto *statements_node = static_cast<const AstlangNode *>(nodes.at(3).get());

        core::ComputationResult condition_result = condition_node->compute(interpreter);
        const astlang2::Value condition_value = std::move(condition_result.value);

        if (condition_value.type != interpreter.getBool()) {
            throw std::runtime_error{"Condition must be a boolean"};
        }

        const bool condition = *static_cast<const bool *>(condition_value.object.get());

        if (condition) {
            core::ComputationResult statement_result = statements_node->compute(interpreter);

            statement_result.controlflowStatus =
                statement_result.controlflowStatus == core::ControlflowStatus::RETURN
                    ? statement_result.controlflowStatus
                    : core::ControlflowStatus::IF_BLOCK_FINISHES;

            return statement_result;
        }

        return core::ComputationResult{
            .controlflowStatus = core::ControlflowStatus::IF_CONDITION_FAILED,
        };
    }
}// namespace astlang2::ast::statement

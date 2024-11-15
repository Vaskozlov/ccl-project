#include <ast-lang-2/ast/statement/else_block.hpp>

namespace astlang2::ast::statement
{
    auto ElseBlock::compute(interpreter::Interpreter &interpreter) const -> core::ComputationResult {
        const auto *statements_node = static_cast<const AstlangNode *>(nodes.at(2).get());
        core::ComputationResult statement_result = statements_node->compute(interpreter);

        statement_result.controlflowStatus = core::ControlflowStatus::IF_BLOCK_FINISHES;
        return statement_result;
    }
}// namespace astlang2::ast::statement

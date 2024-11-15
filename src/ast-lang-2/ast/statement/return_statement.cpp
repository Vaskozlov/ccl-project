#include <ast-lang-2/ast/statement/return_statement.hpp>

namespace astlang2::ast::statement
{
    auto ReturnStatement::compute(interpreter::Interpreter &interpreter) const
        -> core::ComputationResult
    {
        const auto *back_node = static_cast<const AstlangNode *>(back().get());
        core::ComputationResult evaluation_result = back_node->compute(interpreter);

        evaluation_result.controlflowStatus = core::ControlflowStatus::RETURN;
        return evaluation_result;
    }
}// namespace astlang2::ast::statement

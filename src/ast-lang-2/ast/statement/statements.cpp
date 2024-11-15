#include <ast-lang-2/ast/statement/statements.hpp>

namespace astlang2::ast::statement
{
    auto Statements::compute(interpreter::Interpreter &interpreter) const -> core::ComputationResult
    {
        const auto *front_node = static_cast<const AstlangNode *>(front().get());
        core::ComputationResult front_node_result = front_node->compute(interpreter);

        if (front_node_result.controlflowStatus == core::ControlflowStatus::RETURN) {
            return front_node_result;
        }

        if (size() > 1) {
            const auto *back_node = static_cast<const AstlangNode *>(back().get());
            return back_node->compute(interpreter);
        }

        return front_node_result;
    }
}// namespace astlang2::ast::statement

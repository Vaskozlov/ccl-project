#include <ast-lang-2/ast/statement/statement.hpp>

namespace astlang2::ast::statement
{
    auto Statement::compute(interpreter::Interpreter &interpreter) const -> core::ComputationResult
    {
        const auto *front_node = static_cast<const AstlangNode *>(front().get());
        return front_node->compute(interpreter);
    }
}// namespace astlang2::ast::statement

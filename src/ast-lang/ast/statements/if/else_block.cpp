#include <ast-lang/ast/statements/if/else_block.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::statement::if_statement
{
    auto ElseBlock::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        auto block = ConstNodePtr{nodes.at(2)};
        return block.astlangNode->compute(interpreter);
    }
}// namespace astlang::ast::statement::if_statement

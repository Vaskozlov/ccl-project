#include <ast-lang/ast/statements/if/else_block.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::statement::if_statement
{
    auto ElseBlock::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        const auto block = ConstNodePtr{nodes.at(2).get()};
        return block.astlangNode->compute(interpreter);
    }
}// namespace astlang::ast::statement::if_statement

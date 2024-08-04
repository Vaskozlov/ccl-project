#include <ast-lang/ast/expression/bitwise_and.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    using namespace interpreter;

    auto BitwiseAnd::compute(Interpreter &interpreter) -> EvaluationResult
    {
        return computeBinaryExpression(
            interpreter, {
                             {
                                 interpreter.BITWISE_AND,
                                 "__bitwise_and__",
                             },
                         });
    }
}// namespace astlang::ast::expression
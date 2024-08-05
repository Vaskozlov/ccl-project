#include <ast-lang/ast/expression/expression.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    using namespace interpreter;

    auto Expression::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        return computeBinaryExpression(
            interpreter, {
                             {
                                 interpreter.PLUS,
                                 "__addition__",
                             },
                             {
                                 interpreter.MINUS,
                                 "__subtraction__",
                             },
                         });
    }
}// namespace astlang::ast::expression
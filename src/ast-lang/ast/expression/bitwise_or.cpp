#include <ast-lang/ast/expression/bitwise_or.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    using namespace interpreter;

    auto BitwiseOr::compute(Interpreter &interpreter) -> EvaluationResult
    {
        return computeBinaryExpression(
            interpreter,
            {
                {
                    interpreter.BITWISE_OR,
                    "__bitwise_or__",
                },
            });
    }
}// namespace astlang::ast::expression
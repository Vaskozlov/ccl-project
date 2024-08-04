#include <ast-lang/ast/expression/equality.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    using namespace interpreter;

    auto Equality::compute(Interpreter &interpreter) -> EvaluationResult
    {
        return computeBinaryExpression(
            interpreter, {
                             {
                                 interpreter.EQUAL,
                                 "__equal__",
                             },
                             {
                                 interpreter.NOT_EQUAL,
                                 "__not_equal__",
                             },
                         });
    }
}// namespace astlang::ast::expression
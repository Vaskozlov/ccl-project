#include <ast-lang/ast/expression/comparison.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    using namespace interpreter;

    auto Comparison::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        return computeBinaryExpression(
            interpreter, {
                             {
                                 interpreter.LESS,
                                 "__less__",
                             },
                             {
                                 interpreter.LESS_EQUAL,
                                 "__less_equal__",
                             },
                             {
                                 interpreter.GREATER,
                                 "__greater__",
                             },
                             {
                                 interpreter.GREATER_EQUAL,
                                 "__greater_equal__",
                             },
                         });
    }
}// namespace astlang::ast::expression
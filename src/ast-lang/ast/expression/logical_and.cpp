#include <ast-lang/ast/expression/logical_and.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    using namespace interpreter;

    auto LogicalAnd::compute(Interpreter &interpreter) -> EvaluationResult
    {
        return computeBinaryExpression(
            interpreter, {
                             {
                                 interpreter.LOGICAL_AND,
                                 "__logical_and__",
                             },
                         });
    }
}// namespace astlang::ast::expression
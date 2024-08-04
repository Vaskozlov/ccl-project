#include <ast-lang/ast/expression/logical_or.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    using namespace interpreter;

    auto LogicalOr::compute(Interpreter &interpreter) -> EvaluationResult
    {
        return computeBinaryExpression(
            interpreter, {
                             {
                                 interpreter.LOGICAL_OR,
                                 "__logical_or__",
                             },
                         });
    }
}// namespace astlang::ast::expression
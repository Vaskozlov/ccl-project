#include <ast-lang/ast/expression/term.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    using namespace astlang::interpreter;

    auto Term::compute(Interpreter &interpreter) -> EvaluationResult
    {
        return computeBinaryExpression(
            interpreter, {
                             {
                                 interpreter.MULTIPLY,
                                 "__multiply__",
                             },
                             {
                                 interpreter.DIVIDE,
                                 "__divide__",
                             },
                         });
    }
}// namespace astlang::ast::expression
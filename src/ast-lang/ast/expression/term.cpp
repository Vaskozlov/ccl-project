#include <ast-lang/ast/expression/term.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    using namespace astlang::interpreter;

    auto Term::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        return computeBinaryExpression(
            interpreter, {
                             {
                                 interpreter.MULTIPLY,
                                 "__multiplication__",
                             },
                             {
                                 interpreter.DIVIDE,
                                 "__division__",
                             },
                         });
    }
}// namespace astlang::ast::expression
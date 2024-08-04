#include <ast-lang/ast/expression/bitwise_xor.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    using namespace interpreter;

    auto BitwiseXor::compute(Interpreter &interpreter) -> EvaluationResult
    {
        return computeBinaryExpression(
            interpreter, {
                             {
                                 interpreter.BITWISE_XOR,
                                 "__bitwise_xor__",
                             },
                         });
    }
}// namespace astlang::ast::expression
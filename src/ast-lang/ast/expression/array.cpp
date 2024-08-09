#include <ast-lang/ast/expression/array.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::expression
{
    using namespace interpreter;

    auto Array::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        return ConstNodePtr{at(1)}.astlangNode->compute(interpreter);
    }
}// namespace astlang::ast::expression

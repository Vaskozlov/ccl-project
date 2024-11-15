#include <ast-lang/ast/statements/return_statement.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::statement
{
    using namespace astlang::interpreter;

    auto ReturnStatement::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        if (size() == 1) {
            // return EvaluationResult{
                // .value = std::nullopt, .type = interpreter::Type::VOID, .needToReturn = true};
        }

        auto return_value = ConstNodePtr{at(1).get()}.astlangNode->compute(interpreter);
        return_value.needToReturn = true;

        return return_value;
    }
}// namespace astlang::ast::statement
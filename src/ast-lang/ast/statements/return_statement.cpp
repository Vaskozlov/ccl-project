#include <ast-lang/ast/statements/return_statement.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::statement
{
    using namespace astlang::interpreter;

    auto ReturnStatement::compute(Interpreter &interpreter) -> EvaluationResult
    {
        if (this->size() == 1) {
            return EvaluationResult{
                .value = std::nullopt, .type = interpreter::Type::VOID, .needToReturn = true};
        }

        auto return_value = NodePtr{this->at(1)}.astlangNode->compute(interpreter);
        return_value.needToReturn = true;

        return return_value;
    }
}// namespace astlang::ast::statement
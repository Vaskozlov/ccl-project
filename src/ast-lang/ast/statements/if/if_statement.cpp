#include <ast-lang/ast/statements/if/if_statement.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::statement::if_statement
{
    auto IfStatement::compute(Interpreter &interpreter) -> EvaluationResult
    {
        return EvaluationResult{.value = std::nullopt, .type = interpreter::Type::VOID};
    }
}// namespace astlang::ast::statement::if_statement

#include <ast-lang/ast/statements/statements.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::statement
{
    using namespace astlang::interpreter;

    auto Statements::compute(Interpreter &interpreter) -> EvaluationResult
    {
        for (const auto &node : nodes) {
            auto result = NodePtr{node.get()}.astlangNode->compute(interpreter);

            if (result.needToReturn) {
                return result;
            }
        }

        return EvaluationResult{.value = std::nullopt, .type = interpreter::Type::VOID};
    }
}// namespace astlang::ast::statement

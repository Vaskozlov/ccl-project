#include <ast-lang/ast/statements/if/if_statement.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::statement::if_statement
{
    auto IfStatement::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        for (const auto &node : nodes) {
            const auto node_ptr = ConstNodePtr{node};

            if (auto result = node_ptr.astlangNode->compute(interpreter);
                result.type != interpreter::Type::ERROR) {
                return result;
            }
        }

        return EvaluationResult{.value = std::nullopt, .type = interpreter::Type::VOID};
    }
}// namespace astlang::ast::statement::if_statement

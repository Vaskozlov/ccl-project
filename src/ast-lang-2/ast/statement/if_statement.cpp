#include <ast-lang-2/ast/statement/if_statement.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::statement
{
    auto IfStatement::compute(interpreter::Interpreter &interpreter) const
        -> core::ComputationResult
    {
        for (const auto &node : nodes) {
            const auto *statement = static_cast<const AstlangNode *>(node.get());
            core::ComputationResult result = statement->compute(interpreter);

            if (result.controlflowStatus == core::ControlflowStatus::RETURN) {
                return result;
            }

            if (result.controlflowStatus == core::ControlflowStatus::IF_BLOCK_FINISHES) {
                return core::ComputationResult{.value = std::move(result.value)};
            }
        }

        return core::ComputationResult{
            .value =
                Value{
                    .object = nullptr,
                    .type = interpreter.getVoid(),
                },
        };
    }
}// namespace astlang2::ast::statement

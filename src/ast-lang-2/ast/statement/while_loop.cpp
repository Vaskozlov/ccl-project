#include <ast-lang-2/ast/statement/while_loop.hpp>

namespace astlang2::ast::statement
{
    auto WhileLoop::compute(interpreter::Interpreter &interpreter) const -> core::ComputationResult
    {
        const auto *condition_node = static_cast<const AstlangNode *>(at(1).get());
        const auto *body_node = static_cast<const AstlangNode *>(at(3).get());

        while (true) {
            core::ComputationResult condition_result = condition_node->compute(interpreter);

            if (condition_result.controlflowStatus == core::ControlflowStatus::RETURN) {
                return condition_result;
            }

            const auto condition_to_bool =
                interpreter.callFunction("bool", {std::move(condition_result.value)});

            if (!*static_cast<bool *>(condition_to_bool.object.get())) {
                return core::ComputationResult{
                    .value =
                        Value{
                            .object = nullptr,
                            .type = interpreter.getVoid(),
                        },
                };
            }

            core::ComputationResult body_result = body_node->compute(interpreter);

            if (body_result.controlflowStatus == core::ControlflowStatus::RETURN) {
                return body_result;
            }
        }
    }
}// namespace astlang2::ast::statement

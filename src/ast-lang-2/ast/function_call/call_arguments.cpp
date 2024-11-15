#include <ast-lang-2/ast/function_call/call_arguments.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::function::call
{
    auto CallArguments::compute(interpreter::Interpreter &interpreter) const
        -> core::ComputationResult
    {
        const auto *argument_node = static_cast<const AstlangNode *>(front().get());
        auto first_argument = argument_node->compute(interpreter);

        std::shared_ptr<void> arguments_ptr = std::make_shared<std::vector<Value>>();
        auto *arguments = static_cast<std::vector<Value> *>(arguments_ptr.get());

        arguments->emplace_back(std::move(first_argument.value));

        if (size() == 1) {
            return core::ComputationResult{
                .value =
                    Value{
                        .object = std::move(arguments_ptr),
                        .type = interpreter.getBuiltinType(),
                    },
            };
        }

        const auto *next_arguments_node = static_cast<const AstlangNode *>(back().get());
        const core::ComputationResult next_arguments_value =
            next_arguments_node->compute(interpreter);

        auto *next_arguments =
            static_cast<std::vector<Value> *>(next_arguments_value.value.object.get());

        for (auto &next_argument : *next_arguments) {
            arguments->emplace_back(std::move(next_argument));
        }

        return core::ComputationResult{
            .value =
                Value{
                    .object = std::move(arguments_ptr),
                    .type = interpreter.getBuiltinType(),
                },
        };
    }
}// namespace astlang2::ast::function::call

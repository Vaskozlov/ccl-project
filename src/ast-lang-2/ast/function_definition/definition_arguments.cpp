#include <ast-lang-2/ast/function_definition/definition_arguments.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::function::def
{
    auto ArgumentsDefinition::compute(interpreter::Interpreter &interpreter) const
        -> core::ComputationResult
    {
        const auto *argument_node = static_cast<const AstlangNode *>(front().get());
        const core::ComputationResult front_argument_value = argument_node->compute(interpreter);

        auto *front_argument = static_cast<std::pair<ts::Type *, isl::string_view> *>(
            front_argument_value.value.object.get());

        std::shared_ptr<void> arguments_ptr =
            std::make_shared<std::vector<std::pair<ts::Type *, isl::string_view>>>();

        auto *arguments = static_cast<std::vector<std::pair<ts::Type *, isl::string_view>> *>(
            arguments_ptr.get());

        arguments->emplace_back(*front_argument);

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
        auto next_arguments_value = next_arguments_node->compute(interpreter);

        auto *next_arguments_vector =
            static_cast<std::vector<std::pair<ts::Type *, isl::string_view>> *>(
                next_arguments_value.value.object.get());

        for (auto &next_argument : *next_arguments_vector) {
            arguments->emplace_back(next_argument);
        }

        return core::ComputationResult{
            .value =
                Value{
                    .object = std::move(arguments_ptr),
                    .type = interpreter.getBuiltinType(),
                },
        };
    }
}// namespace astlang2::ast::function::def
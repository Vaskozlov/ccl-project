#include <ast-lang-2/ast/function_call/method_call.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::function::call
{
    auto MethodCall::compute(interpreter::Interpreter &interpreter) const -> core::ComputationResult
    {
        const auto *object_node = static_cast<const AstlangNode *>(front().get());
        core::ComputationResult object_value = object_node->compute(interpreter);

        if (object_value.controlflowStatus == core::ControlflowStatus::RETURN) {
            return object_value;
        }

        const auto *function_name_node =
            static_cast<const ccl::parser::ast::Terminal *>(at(2).get());

        const ccl::lexer::Token &function_name_token = function_name_node->getToken();
        const auto function_name_repr = function_name_token.getRepr();

        const auto *function_arguments_node = dynamic_cast<const AstlangNode *>(at(4).get());

        std::vector<Value> function_arguments;

        if (function_arguments_node != nullptr) {
            auto function_arguments_values = function_arguments_node->compute(interpreter).value;

            const auto *args =
                static_cast<std::vector<Value> *>(function_arguments_values.object.get());

            function_arguments = std::move(*args);
        }

        auto method_call_result = object_value.value.type->callMethod(
            interpreter, static_cast<std::string>(function_name_repr), object_value.value,
            std::move(function_arguments));

        return core::ComputationResult{.value = std::move(method_call_result)};
    }
}// namespace astlang2::ast::function::call

#include <ast-lang-2/ast/function_call/function_call.hpp>

namespace astlang2::ast::function::call
{
    auto FunctionCall::compute(interpreter::Interpreter &interpreter) const
        -> core::ComputationResult
    {
        const auto *function_name_node =
            static_cast<const ccl::parser::ast::Terminal *>(front().get());

        const ccl::lexer::Token &function_name_token = function_name_node->getToken();
        const auto function_name_repr = function_name_token.getRepr();

        const auto *function_arguments_node = dynamic_cast<const AstlangNode *>(at(2).get());

        std::vector<Value> function_arguments;

        if (function_arguments_node != nullptr) {
            auto function_arguments_values = function_arguments_node->compute(interpreter).value;

            const auto *args =
                static_cast<std::vector<Value> *>(function_arguments_values.object.get());

            function_arguments = std::move(*args);
        }

        return core::ComputationResult{
            .value = interpreter.callFunction(
                static_cast<std::string>(function_name_repr), function_arguments),
        };
    }
}// namespace astlang2::ast::function::call

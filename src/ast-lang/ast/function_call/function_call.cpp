#include <ast-lang/ast/function_call/function_call.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::function::call
{
    using namespace astlang::interpreter;

    auto FunctionCall::compute(Interpreter &interpreter) const -> EvaluationResult
    {
        const auto front_node = ConstNodePtr{front().get()};
        const auto &front_token = front_node.terminalNode->getToken();
        const auto function_name = front_token.getRepr();

        const auto call_arguments = ConstNodePtr{at(2).get()};
        auto arguments = call_arguments.astlangNode->compute(interpreter);
        auto casted_arguments = isl::get<std::vector<EvaluationResult>>(arguments.value);

        return interpreter.call(std::string{function_name}, std::move(casted_arguments));
    }
}// namespace astlang::ast::function::call
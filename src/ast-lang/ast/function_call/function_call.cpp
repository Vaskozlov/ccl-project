#include <ast-lang/ast/function_call/function_call.hpp>
#include <ast-lang/interpreter/interpreter.hpp>

namespace astlang::ast::function::call
{
    using namespace astlang::interpreter;

    auto FunctionCall::compute(Interpreter &interpreter) -> EvaluationResult
    {
        const auto front_node = NodePtr{this->front()};
        const auto &front_token = front_node.tokenNode->getToken();
        const auto function_name = front_token.getRepr();

        auto call_arguments = NodePtr{this->at(2)};
        auto arguments = call_arguments.astlangNode->compute(interpreter);
        auto casted_arguments = isl::get<std::vector<EvaluationResult>>(arguments.value);

        return interpreter.call(std::string{function_name}, std::move(casted_arguments));
    }
}// namespace astlang::ast::function::call
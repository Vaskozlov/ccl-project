#include <ast-lang-2/ast/function_call/method_call.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::function::call
{
    MethodCall::MethodCall(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
      , objectNode{isl::staticPointerCast<AstlangNode>(initial_nodes.front())}
      , methodNameNode{isl::staticPointerCast<ccl::parser::ast::Terminal>(initial_nodes.at(2))}
    {
        if (initial_nodes.size() == 5) {
            return;
        }

        const auto *function_arguments_node =
            static_cast<const ccl::parser::ast::NonTerminal *>(initial_nodes.at(4).get());

        functionArgumentsNode.emplace_back(
            isl::staticPointerCast<AstlangNode>(function_arguments_node->front()));

        while (function_arguments_node->size() == 3) {
            function_arguments_node = static_cast<const ccl::parser::ast::NonTerminal *>(
                function_arguments_node->back().get());

            functionArgumentsNode.emplace_back(
                isl::staticPointerCast<AstlangNode>(function_arguments_node->front()));
        }
    }

    auto MethodCall::compute(interpreter::Interpreter &interpreter) const -> core::ComputationResult
    {
        core::ComputationResult object_value = objectNode->compute(interpreter);

        if (object_value.controlflowStatus == core::ControlflowStatus::RETURN) {
            return object_value;
        }

        const ccl::lexer::Token &method_name_token = methodNameNode->getToken();
        const auto function_name_repr = method_name_token.getRepr();

        isl::SmallVector<Value, 4> function_arguments{std::move(object_value.value)};

        for (const auto &argument_node : functionArgumentsNode) {
            function_arguments.emplace_back(argument_node->compute(interpreter).value);
        }

        auto method_call_result = object_value.value.type->callMethod(
            interpreter, static_cast<std::string>(function_name_repr), function_arguments);

        return core::ComputationResult{.value = std::move(method_call_result)};
    }

    auto MethodCall::castChildren(const ConversionTable &conversion_table) -> void
    {
        methodNameNode->cast(conversion_table);

        for (auto &node : functionArgumentsNode) {
            node->cast(conversion_table);
        }

        objectNode->cast(conversion_table);
    }

    auto MethodCall::optimize() -> core::SharedNode<>
    {
        return nullptr;
    }
}// namespace astlang2::ast::function::call

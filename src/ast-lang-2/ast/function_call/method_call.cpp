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

    auto MethodCall::compute(interpreter::Interpreter &interpreter) const -> ComputationResult
    {
        ComputationResult object_value = objectNode->compute(interpreter);

        if (object_value.controlflowStatus == ControlflowStatus::RETURN) {
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

        return ComputationResult{.value = std::move(method_call_result)};
    }

    auto MethodCall::optimize() -> SharedNode<>
    {
        return nullptr;
    }

    auto MethodCall::getChildrenNodes() const -> isl::SmallFunction<ccl::parser::ast::SharedNode<>()>
    {
        return isl::SmallFunction<ccl::parser::ast::SharedNode<>()>{
            [this, field_index = 0U, index = 0U]() mutable -> ccl::parser::ast::SharedNode<> {
                switch (field_index) {
                case 0:
                    ++field_index;
                    return objectNode;

                case 1:
                    ++field_index;
                    return methodNameNode;

                case 2:
                    if (index == functionArgumentsNode.size()) {
                        ++field_index;
                        return nullptr;
                    }

                    return functionArgumentsNode[index++];

                default:
                    return nullptr;
                }
            }};
    }
}// namespace astlang2::ast::function::call

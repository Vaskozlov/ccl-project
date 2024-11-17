#include <ast-lang-2/ast/function_call/function_call.hpp>

namespace astlang2::ast::function::call
{
    FunctionCall::FunctionCall(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
      , functionNameNode{isl::staticPointerCast<ccl::parser::ast::Terminal>(initial_nodes.front())}
    {
        if (initial_nodes.size() == 3) {
            return;
        }

        const auto *function_arguments_node =
            static_cast<const ccl::parser::ast::NonTerminal *>(initial_nodes.at(2).get());

        functionArgumentsNode.emplace_back(
            isl::staticPointerCast<AstlangNode>(function_arguments_node->front()));

        while (function_arguments_node->size() == 3) {
            function_arguments_node = static_cast<const ccl::parser::ast::NonTerminal *>(
                function_arguments_node->back().get());

            functionArgumentsNode.emplace_back(
                isl::staticPointerCast<AstlangNode>(function_arguments_node->front()));
        }
    }

    auto FunctionCall::compute(interpreter::Interpreter &interpreter) const
        -> core::ComputationResult
    {
        const ccl::lexer::Token &function_name_token = functionNameNode->getToken();
        const auto function_name_repr = function_name_token.getRepr();

        isl::SmallVector<Value, 4> function_arguments;

        for (const auto &argument_node : functionArgumentsNode) {
            function_arguments.emplace_back(argument_node->compute(interpreter).value);
        }

        return core::ComputationResult{
            .value = interpreter.callFunction(
                static_cast<std::string>(function_name_repr), function_arguments),
        };
    }

    auto FunctionCall::castChildren(const ConversionTable &conversion_table) -> void
    {
        for (auto &node : functionArgumentsNode) {
            node->cast(conversion_table);
        }

        functionNameNode->cast(conversion_table);
    }

    auto FunctionCall::optimize() -> core::SharedNode<>
    {
        return nullptr;
    }
}// namespace astlang2::ast::function::call

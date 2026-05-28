#include <ast-lang-2/ast/function_call/function_call.hpp>
#include <ast-lang-2/ast/expression/factor.hpp>
#include <ast-lang-2/ast/constant_evaluator.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

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
        -> ComputationResult
    {
        const ccl::lexer::Token &function_name_token = functionNameNode->getToken();
        const auto function_name_repr = function_name_token.getRepr();

        isl::SmallVector<Value, 4> function_arguments;

        for (const auto &argument_node : functionArgumentsNode) {
            function_arguments.emplace_back(argument_node->compute(interpreter).value);
        }

        return ComputationResult{
            .value = interpreter.callFunction(
                static_cast<std::string>(function_name_repr), function_arguments),
        };
    }

    auto FunctionCall::optimize() -> SharedNode<>
    {
        for (auto &argument_node : functionArgumentsNode) {
            exchangeIfNotNull(argument_node, argument_node->optimize());
        }

        const auto &funcNameToken = functionNameNode->getToken();
        std::string funcName{static_cast<std::string>(funcNameToken.getRepr())};

        isl::SmallVector<ExtractedConstant, 4> args;
        bool allConstants = true;
        for (auto &argNode : functionArgumentsNode) {
            auto argConst = ConstantEvaluator::tryExtractConstant(argNode);
            if (!argConst) {
                allConstants = false;
                break;
            }
            args.emplace_back(*argConst);
        }

        if (allConstants) {
            auto result = ConstantEvaluator::evaluateBuiltin(funcName, std::move(args));
            if (result) {
                // Store result in first argument's Factor (in-place modification)
                if (!functionArgumentsNode.empty()) {
                    auto *argFactor = dynamic_cast<expression::Factor *>(functionArgumentsNode[0].get());
                    if (argFactor) {
                        argFactor->updateConstant(result->resultType, std::move(result->value));
                        return functionArgumentsNode[0];
                    }
                }
            }
        }

        return nullptr;
    }

    auto FunctionCall::getChildrenNodes() const -> ChildrenNodesGenerator
    {
        return ChildrenNodesGenerator{
            [this, field_index = 0U, index = 0U]() mutable -> ccl::parser::ast::SharedNode<> {
                switch (field_index) {
                case 0:
                    ++field_index;
                    return functionNameNode;

                case 1:
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

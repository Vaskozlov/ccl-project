#include <ast-lang-2/ast/function_definition/function_definition.hpp>
#include <ast-lang-2/function/astlang_function.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::function::def {
    static auto getFunctionArgument(const ccl::parser::ast::NonTerminal* function_argument)
        -> std::pair<std::string, core::SharedNode<ccl::parser::ast::NonTerminal>> {
        const auto* argument_name_node =
                static_cast<const ccl::parser::ast::Terminal *>(function_argument->back().get());
        const auto&argument_name_token = argument_name_node->getToken();
        const auto argument_name_repr = argument_name_token.getRepr();

        return {
            static_cast<std::string>(argument_name_repr),
            isl::staticPointerCast<ccl::parser::ast::NonTerminal>(function_argument->front()),
        };
    }

    FunctionDefinition::FunctionDefinition(
        const SmallId id,
        const ccl::parser::ast::SmallVectorOfNodes&initial_nodes)
        : AstlangNode{id} {
        u32 elements_offset = 0;

        const auto* function_name_node =
                static_cast<const ccl::parser::ast::Terminal *>(initial_nodes.at(1).get());

        const ccl::lexer::Token&function_name_token = function_name_node->getToken();
        const auto function_name_repr = function_name_token.getRepr();

        functionName = static_cast<std::string>(function_name_repr);

        const auto* arguments_nodes =
                dynamic_cast<const ccl::parser::ast::NonTerminal *>(initial_nodes.at(3).get());

        if (arguments_nodes == nullptr) {
            elements_offset += 1;
        }

        const auto* return_type_node = dynamic_cast<ccl::parser::ast::NonTerminal *>(
            initial_nodes.at(5U - elements_offset).get());

        if (return_type_node == nullptr) {
            elements_offset += 1;
        }
        else {
            returnTypeNode =
                    isl::staticPointerCast<ccl::parser::ast::NonTerminal>(return_type_node->back());
        }

        bodyNode = isl::staticPointerCast<AstlangNode>(initial_nodes.at(7U - elements_offset));

        if (arguments_nodes == nullptr) {
            return;
        }

        auto [name, argument] = getFunctionArgument(
            static_cast<const ccl::parser::ast::NonTerminal *>(arguments_nodes->front().get()));

        argumentsNames.emplace_back(std::move(name));
        functionArguments.emplace_back(std::move(argument));

        while (true) {
            if (arguments_nodes->size() < 3) {
                break;
            }

            arguments_nodes =
                    static_cast<const ccl::parser::ast::NonTerminal *>(arguments_nodes->back().get());

            auto [name, argument] = getFunctionArgument(
                static_cast<const ccl::parser::ast::NonTerminal *>(arguments_nodes->front().get()));

            argumentsNames.emplace_back(std::move(name));
            functionArguments.emplace_back(std::move(argument));
        }
    }

    auto FunctionDefinition::compute(interpreter::Interpreter&interpreter) const
        -> core::ComputationResult {
        ts::Type* return_type = returnTypeNode == nullptr
                                    ? interpreter.getAny()
                                    : interpreter.getTypeFromNode(returnTypeNode.get());


        astlang2::function::FunctionIdentification function_identification{
            .name = functionName,
            .returnType = return_type,
        };

        for (const auto&argument_node: functionArguments) {
            function_identification.arguments.emplace_back(
                interpreter.getTypeFromNode(argument_node.get()));
        }

        interpreter.addFunction(
            function_identification,
            std::make_unique<astlang2::function::AstlangFunction>(argumentsNames, bodyNode));

        return core::ComputationResult{
            .value =
            Value{
                .object = nullptr,
                .type = interpreter.getVoid(),
            },
        };
    }

    auto FunctionDefinition::castChildren(const ConversionTable&conversion_table) -> void {
        for (auto&argument_type_node: functionArguments) {
            argument_type_node->cast(conversion_table);
        }

        if (returnTypeNode != nullptr) {
            returnTypeNode->cast(conversion_table);
        }

        bodyNode->cast(conversion_table);
    }

    auto FunctionDefinition::optimize() -> core::SharedNode<> {
        bodyNode->optimize();
        return nullptr;
    }
} // namespace astlang2::ast::function::def

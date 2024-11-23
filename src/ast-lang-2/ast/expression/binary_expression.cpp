#include <ast-lang-2/ast/expression/binary_expression.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::expression
{
    static auto logicalAnd(
        const AstlangNode *lhs_node, const AstlangNode *rhs_node,
        interpreter::Interpreter &interpreter) -> ComputationResult
    {
        ComputationResult lhs_argument = lhs_node->compute(interpreter);

        if (lhs_argument.controlflowStatus == ControlflowStatus::RETURN) {
            return lhs_argument;
        }

        const auto lhs_to_bool = interpreter.callFunction("bool", {std::move(lhs_argument.value)});

        if (!*static_cast<bool *>(lhs_to_bool.object.get())) {
            return ComputationResult{.value = interpreter.BOOL_FALSE};
        }

        ComputationResult rhs_argument = rhs_node->compute(interpreter);

        auto rhs_to_bool = interpreter.callFunction("bool", {std::move(rhs_argument.value)});

        return *static_cast<bool *>(lhs_to_bool.object.get())
                   ? ComputationResult{.value = interpreter.BOOL_TRUE}
                   : ComputationResult{.value = interpreter.BOOL_FALSE};
    }

    static auto logicalOr(
        const AstlangNode *lhs_node, const AstlangNode *rhs_node,
        interpreter::Interpreter &interpreter) -> ComputationResult
    {
        ComputationResult lhs_argument = lhs_node->compute(interpreter);

        if (lhs_argument.controlflowStatus == ControlflowStatus::RETURN) {
            return lhs_argument;
        }

        const auto lhs_to_bool = interpreter.callFunction("bool", {std::move(lhs_argument.value)});

        if (*static_cast<bool *>(lhs_to_bool.object.get())) {
            return ComputationResult{.value = interpreter.BOOL_TRUE};
        }

        ComputationResult rhs_argument = rhs_node->compute(interpreter);

        auto rhs_to_bool = interpreter.callFunction("bool", {std::move(rhs_argument.value)});

        return *static_cast<bool *>(lhs_to_bool.object.get())
                   ? ComputationResult{.value = interpreter.BOOL_TRUE}
                   : ComputationResult{.value = interpreter.BOOL_FALSE};
    }

    BinaryExpression::BinaryExpression(
        const SmallId id, const ccl::parser::ast::SmallVectorOfNodes &initial_nodes)
      : AstlangNode{id}
      , lhsNode{isl::staticPointerCast<AstlangNode>(initial_nodes.front())}
      , rhsNode{
            initial_nodes.size() == 1 ? nullptr
                                      : isl::staticPointerCast<AstlangNode>(initial_nodes.at(2))}
    {
        if (initial_nodes.size() == 1) {
            return;
        }

        const auto *operator_node =
            static_cast<ccl::parser::ast::Terminal *>(initial_nodes.at(1).get());

        switch (static_cast<NodeTypes>(operator_node->getType())) {
            using enum NodeTypes;

        case PLUS:
            functionName = "__add__";
            break;

        case MINUS:
            functionName = "__sub__";
            break;

        case MULTIPLY:
            functionName = "__mul__";
            break;

        case DIVIDE:
            functionName = "__div__";
            break;

        case MODULUS:
            functionName = "__mod__";
            break;

        case LESS:
            functionName = "__less__";
            break;

        case LESS_EQUAL:
            functionName = "__less_eq__";
            break;

        case GREATER:
            functionName = "__greater__";
            break;

        case GREATER_EQUAL:
            functionName = "__greater_eq__";
            break;

        case EQUAL:
            functionName = "__equal__";
            break;

        case NOT_EQUAL:
            functionName = "__not_equal__";
            break;

        case BITWISE_AND:
            functionName = "__bitwise_and__";
            break;

        case BITWISE_OR:
            functionName = "__bitwise_or__";
            break;

        case BITWISE_XOR:
            functionName = "__bitwise__xor__";
            break;

        case LOGICAL_OR:
            functionName = "__logical_or__";
            break;

        case LOGICAL_AND:
            functionName = "__logical_and__";
            break;

        case ASSIGN:
            functionName = "__assign__";
            break;

        default:
            throw std::runtime_error{"Unsupported binary expression"};
        }
    }

    auto BinaryExpression::compute(interpreter::Interpreter &interpreter) const
        -> ComputationResult
    {
        if (rhsNode == nullptr) {
            return lhsNode->compute(interpreter);
        }

        if (functionName == "__logical_and__") {
            return logicalAnd(lhsNode.get(), rhsNode.get(), interpreter);
        }

        if (functionName == "__logical_or__") {
            return logicalOr(lhsNode.get(), rhsNode.get(), interpreter);
        }

        isl::SmallVector<astlang2::Value, 4> function_arguments;
        ComputationResult lhs_argument = lhsNode->compute(interpreter);

        if (lhs_argument.controlflowStatus == ControlflowStatus::RETURN) {
            return lhs_argument;
        }

        ComputationResult rhs_argument = rhsNode->compute(interpreter);

        if (rhs_argument.controlflowStatus == ControlflowStatus::RETURN) {
            return rhs_argument;
        }

        function_arguments.emplace_back(std::move(lhs_argument.value));
        function_arguments.emplace_back(std::move(rhs_argument.value));

        auto expression_result = interpreter.callFunction(functionName, function_arguments);

        return ComputationResult{.value = std::move(expression_result)};
    }

    auto BinaryExpression::optimize() -> SharedNode<>
    {
        exchangeIfNotNull(lhsNode, lhsNode->optimize());

        if (rhsNode == nullptr) {
            return lhsNode;
        }

        exchangeIfNotNull(rhsNode, rhsNode->optimize());

        return nullptr;
    }

    auto BinaryExpression::getChildrenNodes() const
        -> isl::SmallFunction<ccl::parser::ast::SharedNode<>()>
    {
        return isl::SmallFunction<ccl::parser::ast::SharedNode<>()>{
            [this, index = 0]() mutable -> ccl::parser::ast::SharedNode<> {
                switch (index++) {
                case 0:
                    return lhsNode;

                case 1:
                    if (rhsNode == nullptr ) {
                        return nullptr;
                    }

                    return rhsNode;

                default:
                    return nullptr;
                }
            }};
    }

}// namespace astlang2::ast::expression

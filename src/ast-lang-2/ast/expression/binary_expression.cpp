#include <ast-lang-2/ast/expression/binary_expression.hpp>
#include <ast-lang-2/interpreter/interpreter.hpp>

namespace astlang2::ast::expression
{
    static auto logicalAnd(
        const core::AstlangNode *lhs_node, const core::AstlangNode *rhs_node,
        interpreter::Interpreter &interpreter) -> core::ComputationResult
    {
        core::ComputationResult lhs_argument = lhs_node->compute(interpreter);

        if (lhs_argument.controlflowStatus == core::ControlflowStatus::RETURN) {
            return lhs_argument;
        }

        const auto lhs_to_bool = interpreter.callFunction("bool", {std::move(lhs_argument.value)});

        if (!*static_cast<bool *>(lhs_to_bool.object.get())) {
            return core::ComputationResult{.value = interpreter.BOOL_FALSE};
        }

        core::ComputationResult rhs_argument = rhs_node->compute(interpreter);

        auto rhs_to_bool = interpreter.callFunction("bool", {std::move(rhs_argument.value)});

        return *static_cast<bool *>(lhs_to_bool.object.get())
                   ? core::ComputationResult{.value = interpreter.BOOL_TRUE}
                   : core::ComputationResult{.value = interpreter.BOOL_FALSE};
    }

    static auto logicalOr(
        const core::AstlangNode *lhs_node, const core::AstlangNode *rhs_node,
        interpreter::Interpreter &interpreter) -> core::ComputationResult
    {
        core::ComputationResult lhs_argument = lhs_node->compute(interpreter);

        if (lhs_argument.controlflowStatus == core::ControlflowStatus::RETURN) {
            return lhs_argument;
        }

        const auto lhs_to_bool = interpreter.callFunction("bool", {std::move(lhs_argument.value)});

        if (*static_cast<bool *>(lhs_to_bool.object.get())) {
            return core::ComputationResult{.value = interpreter.BOOL_TRUE};
        }

        core::ComputationResult rhs_argument = rhs_node->compute(interpreter);

        auto rhs_to_bool = interpreter.callFunction("bool", {std::move(rhs_argument.value)});

        return *static_cast<bool *>(lhs_to_bool.object.get())
                   ? core::ComputationResult{.value = interpreter.BOOL_TRUE}
                   : core::ComputationResult{.value = interpreter.BOOL_FALSE};
    }

    auto BinaryExpression::compute(interpreter::Interpreter &interpreter) const
        -> core::ComputationResult
    {
        const auto *front_node = static_cast<AstlangNode *>(front().get());

        if (size() == 1) {
            return front_node->compute(interpreter);
        }

        const auto *operator_node = static_cast<ccl::parser::ast::Terminal *>(at(1).get());
        const auto operation = operator_node->getType();
        std::string function_name;

        const auto *back_node = static_cast<const AstlangNode *>(back().get());

        if (operation == interpreter.PLUS) {
            function_name = "__add__";
        } else if (operation == interpreter.MINUS) {
            function_name = "__sub__";
        } else if (operation == interpreter.MULTIPLY) {
            function_name = "__mul__";
        } else if (operation == interpreter.DIVIDE) {
            function_name = "__div__";
        } else if (operation == interpreter.LESS) {
            function_name = "__less__";
        } else if (operation == interpreter.GREATER) {
            function_name = "__greater__";
        } else if (operation == interpreter.LESS_EQUAL) {
            function_name = "__less_eq__";
        } else if (operation == interpreter.GREATER_EQUAL) {
            function_name = "__greater_eq__";
        } else if (operation == interpreter.EQUAL) {
            function_name = "__equal__";
        } else if (operation == interpreter.NOT_EQUAL) {
            function_name = "__not_equal__";
        } else if (operation == interpreter.BITWISE_AND) {
            function_name = "__bitwise_and__";
        } else if (operation == interpreter.BITWISE_OR) {
            function_name = "__bitwise_or__";
        } else if (operation == interpreter.BITWISE_XOR) {
            function_name = "__bitwise_xor__";
        } else if (operation == interpreter.LOGICAL_OR) {
            return logicalOr(front_node, back_node, interpreter);
        } else if (operation == interpreter.LOGICAL_AND) {
            return logicalAnd(front_node, back_node, interpreter);
        } else {
            throw std::runtime_error{"Unsupported binary expression"};
        }

        std::vector<astlang2::Value> function_arguments;
        function_arguments.reserve(2);

        core::ComputationResult lhs_argument = front_node->compute(interpreter);

        if (lhs_argument.controlflowStatus == core::ControlflowStatus::RETURN) {
            return lhs_argument;
        }

        core::ComputationResult rhs_argument = back_node->compute(interpreter);

        if (rhs_argument.controlflowStatus == core::ControlflowStatus::RETURN) {
            return rhs_argument;
        }

        function_arguments.emplace_back(std::move(lhs_argument.value));
        function_arguments.emplace_back(std::move(rhs_argument.value));

        auto expression_result = interpreter.callFunction(function_name, function_arguments);

        return core::ComputationResult{.value = std::move(expression_result)};
    }
}// namespace astlang2::ast::expression
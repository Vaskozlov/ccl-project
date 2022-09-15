#ifndef CCL_PROJECT_BINARY_EXPRESSION_HPP
#define CCL_PROJECT_BINARY_EXPRESSION_HPP

#include <fsc/parser/nodes/basic.hpp>

namespace fsc::ast
{
    struct BinaryExpressionNode : BasicNode
    {
        explicit BinaryExpressionNode(
            Token &token_, unique_ptr<BasicNode> left_ = nullptr, unique_ptr<BasicNode> right_ = nullptr)
          : binary_op{ token_ }, left{ std::move(left_) }, right{ std::move(right_) }
        {}

        auto print(const std::string &prefix, bool isLeft) const -> void override
        {
            fmt::print(
                "{}{}{}\n", prefix, isLeft ? "\u251c\u2500\u2500" : "\u2514\u2500\u2500",
                binary_op.getRepr());

            left->print(prefix + (isLeft ? "\u2502   " : "    "), true);
            right->print(prefix + (isLeft ? "\u2502   " : "    "), false);
        }

        Token binary_op{};
        unique_ptr<BasicNode> left{};
        unique_ptr<BasicNode> right{};
    };
}// namespace fsc::ast::node

#endif /* CCL_PROJECT_BINARY_EXPRESSION_HPP */

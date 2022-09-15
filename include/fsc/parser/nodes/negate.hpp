#ifndef CCL_PROJECT_NEGATE_HPP
#define CCL_PROJECT_NEGATE_HPP

#include <fsc/parser/nodes/basic.hpp>

namespace fsc::ast
{
    struct NegateNode : BasicNode
    {
        explicit NegateNode(unique_ptr<BasicNode> node_) : node{ std::move(node_) }
        {}

        auto print(const std::string &prefix, bool isLeft) const -> void override
        {
            fmt::print(
                "{}{}Negate\n", prefix, isLeft ? "\u251c\u2500\u2500" : "\u2514\u2500\u2500");
            node->print(prefix + (isLeft ? "\u2502   " : "    "), false);
        }

        unique_ptr<BasicNode> node{};
    };
}// namespace fsc::ast

#endif /* CCL_PROJECT_NEGATE_HPP */

#ifndef CCL_PROJECT_PARAMETER_HPP
#define CCL_PROJECT_PARAMETER_HPP

#include <fsc/parser/nodes/basic.hpp>

namespace fsc::ast
{
    struct ParameterNode : BasicNode
    {
        CCL_PERFECT_FORWARDING(T, Token)
        explicit ParameterNode(T &&name_, unique_ptr<BasicNode> expression_ = nullptr)
          : name{ std::forward<T>(name_) }, expression{ std::move(expression_) }
        {}

        auto print(const std::string &prefix, bool isLeft) const -> void override
        {
            fmt::print(
                "{}{}Parameter: {}\n", prefix, isLeft ? "\u251c\u2500\u2500" : "\u2514\u2500\u2500",
                name.getRepr());
            expression->print(prefix + (isLeft ? "\u2502   " : "    "), false);
        }

        Token name{};
        unique_ptr<BasicNode> expression{};
    };
}// namespace fsc::ast

#endif /* CCL_PROJECT_PARAMETER_HPP */

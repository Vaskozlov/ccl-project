#ifndef CCL_PROJECT_VARIABLE_DECL_HPP
#define CCL_PROJECT_VARIABLE_DECL_HPP

#include <fsc/parser/nodes/basic.hpp>


namespace fsc::ast
{
    struct VariableDeclNode : BasicNode
    {
        CCL_PERFECT_FORWARDING_2(T1, Token, T2, Token)
        explicit VariableDeclNode(T1 &&name_, T2 &&type, unique_ptr<BasicNode> initializer = nullptr)
          : initializer(std::move(initializer)), name(std::forward<T1>(name_)),
            type(std::forward<T2>(type))
        {}

        auto print(const std::string &prefix, bool isLeft) const -> void override
        {
            fmt::print(
                "{}{}{}: {}\n", prefix, isLeft ? "\u251c\u2500\u2500" : "\u2514\u2500\u2500",
                name.getRepr(), type.getRepr());

            if (initializer) {
                initializer->print(prefix + (isLeft ? "\u2502  " : "   "), false);
            }
        }

        unique_ptr<BasicNode> initializer{};
        Token name{};
        Token type{};
    };
}// namespace fsc::ast

#endif /* CCL_PROJECT_VARIABLE_DECL_HPP */

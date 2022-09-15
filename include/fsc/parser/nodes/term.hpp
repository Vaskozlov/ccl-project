#ifndef CCL_PROJECT_TERM_HPP
#define CCL_PROJECT_TERM_HPP

#include <fsc/parser/nodes/basic.hpp>

namespace fsc::ast
{
    struct TermNode : BasicNode
    {
        CCL_PERFECT_FORWARDING(T, Token)
        explicit TermNode(T &&token_) : token{ std::forward<T>(token_) }
        {}

        auto print(const std::string &prefix, bool isLeft) const -> void override
        {
            fmt::print(
                "{}{}{}\n", prefix, isLeft ? "\u251c\u2500\u2500" : "\u2514\u2500\u2500",
                token.getRepr());
        }

        Token token{};
    };
}// namespace fsc::ast::node

#endif /* CCL_PROJECT_TERM_HPP */

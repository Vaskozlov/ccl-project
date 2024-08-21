#ifndef CCL_PROJECT_RULE_WITH_DOT_HPP
#define CCL_PROJECT_RULE_WITH_DOT_HPP

#include <ccl/parser/rule.hpp>

namespace ccl::parser
{
    struct RuleWithDot
    {
        const Rule *rule{};
        SmallId dotPosition{};

        [[nodiscard]] auto
            operator<=>(const RuleWithDot &other) const noexcept -> std::weak_ordering = default;

        [[nodiscard]] auto begin() const -> decltype(auto)
        {
            return rule->begin();
        }

        [[nodiscard]] auto end() const -> decltype(auto)
        {
            return rule->end();
        }

        [[nodiscard]] auto isDotInTheEnd() const noexcept -> bool
        {
            return dotPosition == rule->size();
        }

        [[nodiscard]] auto at(std::size_t index) const -> Symbol
        {
            return rule->at(index);
        }

        [[nodiscard]] auto atDot() const -> Symbol
        {
            return rule->at(dotPosition);
        }

        [[nodiscard]] auto size() const noexcept -> std::size_t
        {
            return rule->size();
        }
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_RULE_WITH_DOT_HPP */

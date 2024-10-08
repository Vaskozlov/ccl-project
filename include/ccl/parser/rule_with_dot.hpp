#ifndef CCL_PROJECT_RULE_WITH_DOT_HPP
#define CCL_PROJECT_RULE_WITH_DOT_HPP

#include <ccl/parser/rule.hpp>

namespace ccl::parser
{
    struct RuleWithDot
    {
        const Rule *rule{};
        std::size_t dotPosition{};

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
            return dotPosition >= rule->size();
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

    struct RuleWithDotPrintWrapper
    {
        std::function<std::string(SmallId)> idToStringConverter;
        RuleWithDot ruleWithDot;
    };
}// namespace ccl::parser

template<>
struct fmt::formatter<ccl::parser::RuleWithDotPrintWrapper> : formatter<std::string_view>
{
    static auto format(
        const ccl::parser::RuleWithDotPrintWrapper &item_print_wrapper,
        format_context &ctx) -> format_context::iterator;
};

#endif /* CCL_PROJECT_RULE_WITH_DOT_HPP */

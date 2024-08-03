#ifndef CCL_PROJECT_LR_ITEM_HPP
#define CCL_PROJECT_LR_ITEM_HPP

#include "ccl/parser/rule.hpp"
#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/types.hpp>

namespace ccl::parser
{
    struct LrItem
    {
        const Rule *rule;
        std::size_t dotLocation{};
        Symbol production{};
        Symbol lookAhead{};

        [[nodiscard]] auto begin() const -> decltype(auto)
        {
            return rule->begin();
        }

        [[nodiscard]] auto end() const -> decltype(auto)
        {
            return rule->end();
        }

        [[nodiscard]] auto getProductionType() const noexcept -> Symbol
        {
            return production;
        }

        [[nodiscard]] auto getDotLocation() const noexcept -> std::size_t
        {
            return dotLocation;
        }

        [[nodiscard]] auto getLookAhead() const noexcept -> Symbol
        {
            return lookAhead;
        }

        [[nodiscard]] auto getRulePtr() const noexcept CCL_LIFETIMEBOUND -> const Rule *
        {
            return rule;
        }

        [[nodiscard]] auto isDotInTheEnd() const noexcept -> bool
        {
            return dotLocation >= rule->size();
        }

        [[nodiscard]] auto at(std::size_t index) const CCL_LIFETIMEBOUND -> Symbol
        {
            return rule->at(index);
        }

        [[nodiscard]] auto size() const noexcept -> std::size_t
        {
            return rule->size();
        }

        [[nodiscard]] auto
            operator<=>(const LrItem &other) const noexcept -> std::weak_ordering = default;
    };

    struct LrItemPrintWrapper
    {
        const LrItem &item;
        std::function<std::string(SmallId)> idToStr;
    };
}// namespace ccl::parser

template<>
struct std::hash<ccl::parser::LrItem>
{
    auto operator()(const ccl::parser::LrItem &item) const -> std::size_t
    {
        return isl::hash::combine(
            reinterpret_cast<std::size_t>(item.rule), item.dotLocation, item.lookAhead);
    }
};

template<>
class fmt::formatter<ccl::parser::LrItemPrintWrapper> : public fmt::formatter<std::string_view>
{
public:
    auto format(const ccl::parser::LrItemPrintWrapper &item_print_wrapper, format_context &ctx)
        const -> typename format_context::iterator;
};

#endif /* CCL_PROJECT_LR_ITEM_HPP */

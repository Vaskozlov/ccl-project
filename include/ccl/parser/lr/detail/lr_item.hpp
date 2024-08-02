#ifndef CCL_PROJECT_LR_ITEM_HPP
#define CCL_PROJECT_LR_ITEM_HPP

#include "ccl/parser/rule.hpp"
#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/types.hpp>

namespace ccl::parser
{
    class LrItem
    {
    private:
        const Rule *rule;
        std::size_t dotLocation{};
        Symbol production{};
        Symbol lookAhead{};

    public:
        explicit LrItem(
            const Rule *item_rule,
            std::size_t dot_location,
            Symbol item_name,
            Symbol look_ahead)
          : rule{std::move(item_rule)}
          , dotLocation{std::min(dot_location, rule->size())}
          , production{item_name}
          , lookAhead{look_ahead}
        {}

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

        [[nodiscard]] auto getRule() const noexcept CCL_LIFETIMEBOUND -> const Rule &
        {
            return *rule;
        }

        [[nodiscard]] auto isDotInTheEnd() const noexcept -> bool
        {
            return dotLocation == rule->size();
        }

        [[nodiscard]] auto at(std::size_t index) const CCL_LIFETIMEBOUND -> Symbol
        {
            return rule->at(index);
        }

        [[nodiscard]] auto size() const noexcept -> std::size_t
        {
            return rule->size();
        }

        [[nodiscard]] auto operator==(const LrItem &other) const noexcept -> bool
        {
            return dotLocation == other.dotLocation && lookAhead == other.lookAhead &&
                   production == other.production && *rule == *other.rule;
        }

        [[nodiscard]] auto operator<=>(const LrItem &other) const noexcept -> std::weak_ordering
        {
            if (const auto cmp = dotLocation <=> other.dotLocation; cmp != 0) {
                return cmp;
            }

            if (const auto cmp = production <=> other.production; cmp != 0) {
                return cmp;
            }

            if (const auto cmp = lookAhead <=> other.lookAhead; cmp != 0) {
                return cmp;
            }

            return *rule <=> *other.rule;
        }
    };

    struct LrItemPrintWrapper
    {
        const LrItem &item;
        std::function<std::string(Id)> idToStr;
    };
}// namespace ccl::parser

template<>
class fmt::formatter<ccl::parser::LrItemPrintWrapper> : public fmt::formatter<std::string_view>
{
public:
    template<typename FmtContext>
    auto format(const ccl::parser::LrItemPrintWrapper &item_print_wrapper, FmtContext &ctx) const
    {
        const auto &item = item_print_wrapper.item;
        fmt::format_to(ctx.out(), "{} -> [", item_print_wrapper.idToStr(item.getProductionType()));

        for (std::size_t i = 0; i != item.size(); ++i) {
            if (i == item.getDotLocation()) {
                fmt::format_to(ctx.out(), "\u2022");
            }

            fmt::format_to(ctx.out(), "{} ", item_print_wrapper.idToStr(item.at(i)));
        }

        if (item.isDotInTheEnd()) {
            fmt::format_to(ctx.out(), "\u2022");
        }

        return fmt::format_to(ctx.out(), ", {}]", item_print_wrapper.idToStr(item.getLookAhead()));
    }
};

#endif /* CCL_PROJECT_LR_ITEM_HPP */

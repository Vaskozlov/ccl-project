#ifndef CCL_PROJECT_LR_ITEM_HPP
#define CCL_PROJECT_LR_ITEM_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/lr/rule.hpp>
#include <ccl/parser/types.hpp>

namespace ccl::parser
{
    class LrItem
    {
    private:
        Rule rule;
        std::size_t dotLocation{};
        Symbol production{};
        Symbol lookAhead{};
        std::size_t itemHash{};

    public:
        explicit LrItem(
            Rule item_rule,
            std::size_t dot_location,
            Symbol item_name,
            Symbol look_ahead)
          : rule{std::move(item_rule)}
          , dotLocation{std::min(dot_location, rule.size())}
          , production{item_name}
          , lookAhead{look_ahead}
          , itemHash{isl::hash::combine(rule.getHash(), dotLocation, production, lookAhead)}
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
            return rule;
        }

        [[nodiscard]] auto getHash() const noexcept -> std::size_t
        {
            return itemHash;
        }

        [[nodiscard]] auto isDotInTheEnd() const noexcept -> bool
        {
            return dotLocation == rule.size();
        }

        [[nodiscard]] auto at(std::size_t index) const CCL_LIFETIMEBOUND -> Symbol
        {
            return rule.at(index);
        }

        [[nodiscard]] auto size() const noexcept -> std::size_t
        {
            return rule.size();
        }

        [[nodiscard]] auto
            operator<=>(const LrItem &other) const noexcept -> std::weak_ordering = default;
    };

    template<typename T>
    struct LrItemPrintWrapper
    {
        const LrItem &item;
    };
}// namespace ccl::parser

template<>
struct std::hash<ccl::parser::LrItem>
{
    auto operator()(const ccl::parser::LrItem &item) const noexcept -> std::size_t
    {
        return item.getHash();
    }
};

template<typename T>
class fmt::formatter<ccl::parser::LrItemPrintWrapper<T>> : public fmt::formatter<std::string_view>
{
public:
    template<typename FmtContext>
    constexpr auto
        format(const ccl::parser::LrItemPrintWrapper<T> &item_print_wrapper, FmtContext &ctx) const
    {
        const auto &item = item_print_wrapper.item;
        fmt::format_to(
            ctx.out(),
            "{} -> [",
            ccl::lexer::lexerEnumToString(isl::as<T>(item.getProductionType())));

        for (std::size_t i = 0; i != item.size(); ++i) {
            if (i == item.getDotLocation()) {
                fmt::format_to(ctx.out(), "\u2022");
            }

            fmt::format_to(ctx.out(), "{} ", isl::as<T>(item.at(i)));
        }

        if (item.isDotInTheEnd()) {
            fmt::format_to(ctx.out(), "\u2022");
        }

        return fmt::format_to(
            ctx.out(), ", {}]", ccl::lexer::lexerEnumToString(isl::as<T>(item.getLookAhead())));
    }
};

#endif /* CCL_PROJECT_LR_ITEM_HPP */

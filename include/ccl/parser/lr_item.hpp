#ifndef CCL_PROJECT_LR_ITEM_HPP
#define CCL_PROJECT_LR_ITEM_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/general.hpp>

namespace ccl::parser
{
    class LrItem
    {
    private:
        isl::Vector<Production> rule;
        std::size_t dotLocation{};
        Production product{};
        Production lookAhead{};

    public:
        explicit LrItem(
            const isl::Vector<Production> &item_rule,
            std::size_t dot_location,
            Production item_name,
            Production look_ahead)
          : rule{item_rule}
          , dotLocation{std::min(dot_location, rule.size())}
          , product{item_name}
          , lookAhead{look_ahead}
        {}

        [[nodiscard]] auto getProductionType() const noexcept -> Production
        {
            return product;
        }

        [[nodiscard]] auto getDotLocation() const noexcept -> std::size_t
        {
            return dotLocation;
        }

        [[nodiscard]] auto getLookAhead() const noexcept -> Production
        {
            return lookAhead;
        }

        [[nodiscard]] auto
            getRule() const noexcept CCL_LIFETIMEBOUND -> const isl::Vector<Production> &
        {
            return rule;
        }

        [[nodiscard]] auto isDotInTheEnd() const noexcept -> bool
        {
            return dotLocation == rule.size();
        }

        [[nodiscard]] auto at(std::size_t index) CCL_LIFETIMEBOUND -> Production &
        {
            return rule.at(index);
        }

        [[nodiscard]] auto at(std::size_t index) const CCL_LIFETIMEBOUND -> const Production &
        {
            return rule.at(index);
        }

        [[nodiscard]] auto length() const noexcept -> std::size_t
        {
            return rule.size();
        }

        [[nodiscard]] auto
            operator<=>(const LrItem &other) const noexcept -> std::strong_ordering = default;
    };

    template<typename T>
    struct LrItemPrintWrapper
    {
        const LrItem &item;
    };
}// namespace ccl::parser

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

        for (std::size_t i = 0; i != item.length(); ++i) {
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

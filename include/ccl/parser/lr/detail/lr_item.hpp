#ifndef CCL_PROJECT_LR_ITEM_HPP
#define CCL_PROJECT_LR_ITEM_HPP

#include <ankerl/unordered_dense.h>
#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/rule.hpp>
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
struct ankerl::unordered_dense::hash<ccl::parser::LrItem>
{
    using is_avalanching = void;

    [[nodiscard]] auto operator()(const ccl::parser::LrItem &item) const noexcept -> auto
    {
        return ankerl::unordered_dense::detail::wyhash::hash(&item, sizeof(item));
    }
};

template<typename T>
struct ankerl::unordered_dense::hash<std::vector<T>>
{
    using is_avalanching = void;

    [[nodiscard]] auto operator()(const std::vector<T> &vector) const noexcept -> auto
    {
        return ankerl::unordered_dense::detail::wyhash::hash(
            vector.data(), sizeof(T) * vector.size());
    }
};

template<typename T>
struct std::hash<std::vector<T>>
{
    [[nodiscard]] auto operator()(const std::vector<T> &vector) const noexcept -> auto
    {
        return ankerl::unordered_dense::hash<std::vector<T>>{}(vector);
    }
};

template<>
struct std::hash<ccl::parser::LrItem>
{
    auto operator()(const ccl::parser::LrItem &item) const -> auto
    {
        return ankerl::unordered_dense::hash<ccl::parser::LrItem>{}(item);
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

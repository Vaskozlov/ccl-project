#ifndef CCL_PROJECT_LR_ITEM_HPP
#define CCL_PROJECT_LR_ITEM_HPP

#include <ankerl/unordered_dense.h>
#include <ccl/parser/rule_with_dot.hpp>
#include <ccl/parser/types.hpp>

namespace ccl::parser
{
    struct LrItem
    {
        RuleWithDot dottedRule{};
        Symbol production{};
        Symbol lookAhead{};

        [[nodiscard]] auto begin() const -> decltype(auto)
        {
            return dottedRule.begin();
        }

        [[nodiscard]] auto end() const -> decltype(auto)
        {
            return dottedRule.end();
        }

        [[nodiscard]] auto getProductionType() const noexcept -> Symbol
        {
            return production;
        }

        [[nodiscard]] auto getLookAhead() const noexcept -> Symbol
        {
            return lookAhead;
        }

        [[nodiscard]] auto
            operator<=>(const LrItem &other) const noexcept -> std::weak_ordering = default;
    };

    struct LrItemPrintWrapper
    {
        const LrItem &item;
        std::function<std::string(SmallId)> idToStringConverter;
    };
}// namespace ccl::parser

template<>
struct ankerl::unordered_dense::hash<ccl::parser::LrItem>
{
    using is_avalanching = void;

    [[nodiscard]] auto operator()(const ccl::parser::LrItem &item) const noexcept -> auto
    {
        return detail::wyhash::hash(&item, sizeof(item));
    }
};

template<typename T>
struct ankerl::unordered_dense::hash<std::vector<T>>
{
    using is_avalanching = void;

    [[nodiscard]] auto operator()(const std::vector<T> &vector) const noexcept -> auto
    {
        static_assert(std::has_unique_object_representations_v<T>);
        return detail::wyhash::hash(vector.data(), sizeof(T) * vector.size());
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
    auto operator()(const ccl::parser::LrItem &item) const noexcept -> auto
    {
        static_assert(std::has_unique_object_representations_v<ccl::parser::LrItem>);
        return ankerl::unordered_dense::hash<ccl::parser::LrItem>{}(item);
    }
};

template<>
struct fmt::formatter<ccl::parser::LrItemPrintWrapper> : formatter<std::string_view>
{
    static auto format(
        const ccl::parser::LrItemPrintWrapper &item_print_wrapper,
        format_context &ctx) -> format_context::iterator;
};

#endif /* CCL_PROJECT_LR_ITEM_HPP */

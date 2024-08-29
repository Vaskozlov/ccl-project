#ifndef CCL_PROJECT_TABLE_ENTRY_HPP
#define CCL_PROJECT_TABLE_ENTRY_HPP

#include <ankerl/unordered_dense.h>
#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/types.hpp>

namespace ccl::parser
{
    struct TableEntry
    {
        State state{};
        Symbol symbol{};

        [[nodiscard]] auto
            operator<=>(const TableEntry &other) const noexcept -> std::strong_ordering = default;
    };

    template<typename T>
    struct TableEntryPrintHelper
    {
        const TableEntry &entry;
    };
}// namespace ccl::parser

template<>
struct ankerl::unordered_dense::hash<ccl::parser::TableEntry>
{
    using is_avalanching = void;

    [[nodiscard]] auto operator()(const ccl::parser::TableEntry &entry) const noexcept -> auto
    {
        return detail::wyhash::hash(&entry, sizeof(entry));
    }
};

template<>
struct std::hash<ccl::parser::TableEntry>
{
    [[nodiscard]] auto operator()(const ccl::parser::TableEntry &entry) const noexcept -> uint64_t
    {
        return ankerl::unordered_dense::detail::wyhash::hash(&entry, sizeof(entry));
    }
};

template<typename T>
struct fmt::formatter<ccl::parser::TableEntryPrintHelper<T>> : formatter<std::string_view>
{
    constexpr auto format(
        ccl::parser::TableEntryPrintHelper<T> entry_print_helper,
        format_context &ctx) const -> format_context::iterator
    {
        const auto &entry = entry_print_helper.entry;

        return fmt::format_to(
            ctx.out(), "{}-{}", entry.state, ccl::lexer::lexerEnumToString<T>(entry.symbol));
    }
};

#endif /* CCL_PROJECT_TABLE_ENTRY_HPP */

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

    struct TableEntryPrintHelper
    {
        std::function<std::string(SmallId)> idToStringConverter;
        TableEntry entry;
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
        return ankerl::unordered_dense::hash<ccl::parser::TableEntry>{}(entry);
    }
};

template<>
struct fmt::formatter<ccl::parser::TableEntryPrintHelper> : formatter<std::string_view>
{
    static auto format(
        const ccl::parser::TableEntryPrintHelper &entry_print_helper,
        format_context &ctx) -> format_context::iterator
    {
        const auto &[state, symbol] = entry_print_helper.entry;

        return fmt::format_to(
            ctx.out(), "{}-{}", state, entry_print_helper.idToStringConverter(symbol));
    }
};

#endif /* CCL_PROJECT_TABLE_ENTRY_HPP */

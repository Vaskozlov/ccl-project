#ifndef CCL_PROJECT_TABLE_ENTRY_HPP
#define CCL_PROJECT_TABLE_ENTRY_HPP

#include <ccl/parser/types.hpp>

namespace ccl::parser
{
    struct TableEntry
    {
        State state{};
        Symbol lookAhead{};

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
class std::hash<ccl::parser::TableEntry>
{
public:
    auto operator()(const ccl::parser::TableEntry &entry) const -> std::size_t
    {
        return isl::hash::combine(entry.state, entry.lookAhead);
    }
};

template<typename T>
class fmt::formatter<ccl::parser::TableEntryPrintHelper<T>>
  : public fmt::formatter<std::string_view>
{
public:
    template<typename FmtContext>
    constexpr auto
        format(ccl::parser::TableEntryPrintHelper<T> entry_print_helper, FmtContext &ctx) const
    {
        const auto &entry = entry_print_helper.entry;

        return fmt::format_to(
            ctx.out(), "{}-{}", entry.state,
            ccl::lexer::lexerEnumToString(isl::as<T>(entry.lookAhead)));
    }
};

#endif /* CCL_PROJECT_TABLE_ENTRY_HPP */

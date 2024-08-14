#include <ccl/parser/grammar_slot.hpp>

auto fmt::formatter<ccl::parser::GrammarSlotPrintWrapper>::format(
    const ccl::parser::GrammarSlotPrintWrapper &item_print_wrapper, format_context &ctx) const ->
    typename format_context::iterator
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

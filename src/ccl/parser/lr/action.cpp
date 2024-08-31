#include <ccl/parser/lr/action.hpp>

auto fmt::formatter<ccl::parser::ActionPrintWrapper>::format(
    const ccl::parser::ActionPrintWrapper &action_print_wrapper,
    format_context &ctx) -> format_context::iterator
{
    using namespace ccl::parser;

    const auto &action = action_print_wrapper.action;
    fmt::format_to(ctx.out(), "{} ", action.parsingAction);

    if (action.isShift()) {
        return fmt::format_to(ctx.out(), "{}", action.shiftingState);
    }

    if (action.isReduce()) {
        return fmt::format_to(
            ctx.out(), "{}", action_print_wrapper.idToStringConverter(action.productionType));
    }

    return fmt::format_to(ctx.out(), "{}", std::monostate{});
}

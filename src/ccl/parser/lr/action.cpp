#include <ccl/parser/lr/action.hpp>

template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

auto fmt::formatter<ccl::parser::ActionPrintWrapper>::format(
    const ccl::parser::ActionPrintWrapper&action_print_wrapper,
    format_context&ctx) const -> format_context::iterator {
    using namespace ccl::parser;

    const auto&action = action_print_wrapper.action;
    fmt::format_to(ctx.out(), "{} ", action.getParsingAction());

    return std::visit(
        overloaded{
            [&ctx, &action_print_wrapper](const LrItem&arg) {
                return fmt::format_to(
                    ctx.out(), "{}", LrItemPrintWrapper(arg, action_print_wrapper.idToStringConverter));
            },
            [&ctx](auto arg) {
                return fmt::format_to(ctx.out(), "{}", arg);
            },
        },
        action.getStoredData());
}

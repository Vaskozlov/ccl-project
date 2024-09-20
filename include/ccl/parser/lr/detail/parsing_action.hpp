#ifndef CCL_PROJECT_ACTIONS_HPP
#define CCL_PROJECT_ACTIONS_HPP

#include <ccl/ccl.hpp>

namespace ccl::parser
{
    enum class ParsingAction : u8
    {
        SHIFT,
        REDUCE,
        ACCEPT,
        FAILED,
        JOINED,
    };
}

template<>
struct fmt::formatter<ccl::parser::ParsingAction> : formatter<std::string_view>
{
    static constexpr auto format(const ccl::parser::ParsingAction action, format_context &ctx)
        -> format_context::iterator
    {
        switch (action) {
        case ccl::parser::ParsingAction::SHIFT:
            return fmt::format_to(ctx.out(), "SHIFT");

        case ccl::parser::ParsingAction::REDUCE:
            return fmt::format_to(ctx.out(), "REDUCE");

        case ccl::parser::ParsingAction::ACCEPT:
            return fmt::format_to(ctx.out(), "ACCEPT");

        case ccl::parser::ParsingAction::FAILED:
            return fmt::format_to(ctx.out(), "FAILED");

        case ccl::parser::ParsingAction::JOINED:
            return fmt::format_to(ctx.out(), "JOINED");

        default:
            isl::unreachable();
        }
    }
};

#endif /* CCL_PROJECT_ACTIONS_HPP */

#ifndef CCL_PROJECT_ACTIONS_HPP
#define CCL_PROJECT_ACTIONS_HPP

#include <ccl/ccl.hpp>

namespace ccl::parser
{
    enum class ParsingAction : u32
    {
        SHIFT,
        REDUCE,
        ACCEPT
    };
}

template<>
class fmt::formatter<ccl::parser::ParsingAction> : public fmt::formatter<std::string_view>
{
public:
    template<typename FmtContext>
    constexpr auto
        format(ccl::parser::ParsingAction action, FmtContext &ctx) const -> decltype(auto)
    {
        switch (action) {
        case ccl::parser::ParsingAction::SHIFT:
            return fmt::format_to(ctx.out(), "SHIFT");

        case ccl::parser::ParsingAction::REDUCE:
            return fmt::format_to(ctx.out(), "REDUCE");

        case ccl::parser::ParsingAction::ACCEPT:
            return fmt::format_to(ctx.out(), "ACCEPT");

        default:
            std::unreachable();
        }
    }
};

#endif /* CCL_PROJECT_ACTIONS_HPP */

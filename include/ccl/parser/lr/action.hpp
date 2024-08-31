#ifndef CCL_PROJECT_ACTION_HPP
#define CCL_PROJECT_ACTION_HPP

#include <ccl/parser/lr/detail/parsing_action.hpp>
#include <ccl/parser/lr/lr_item.hpp>

namespace ccl::parser
{
    struct Action
    {
        ParsingAction parsingAction;
        State shiftingState{};
        Symbol productionType{};
        SmallId numberOfElementsInProduction{};

        [[nodiscard]] auto
            operator<=>(const Action &other) const noexcept -> std::weak_ordering = default;

        [[nodiscard]] auto isShift() const noexcept -> bool
        {
            return parsingAction == ParsingAction::SHIFT;
        }

        [[nodiscard]] auto isReduce() const noexcept -> bool
        {
            return parsingAction == ParsingAction::REDUCE;
        }

        [[nodiscard]] auto isAccept() const noexcept -> bool
        {
            return parsingAction == ParsingAction::ACCEPT;
        }
    };

    struct ActionPrintWrapper
    {
        const Action &action;
        std::function<std::string(SmallId)> idToStringConverter;
    };
}// namespace ccl::parser

template<>
struct fmt::formatter<ccl::parser::ActionPrintWrapper> : formatter<std::string_view>
{
    static auto format(
        const ccl::parser::ActionPrintWrapper &action_print_wrapper,
        format_context &ctx) -> format_context::iterator;
};

#endif /* CCL_PROJECT_ACTION_HPP */

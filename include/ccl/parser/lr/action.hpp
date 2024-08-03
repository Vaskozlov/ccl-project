#ifndef CCL_PROJECT_ACTION_HPP
#define CCL_PROJECT_ACTION_HPP

#include <ccl/parser/lr/detail/lr_item.hpp>
#include <ccl/parser/lr/detail/parsing_action.hpp>

namespace ccl::parser
{
    class Action
    {
    private:
        std::variant<std::monostate, State, LrItem> data;
        ParsingAction parsingAction;

    public:
        explicit Action(State state)
          : data{state}
          , parsingAction{ParsingAction::SHIFT}
        {}

        explicit Action(const LrItem &item)
          : data{item}
          , parsingAction{ParsingAction::REDUCE}
        {}

        explicit Action(std::monostate accept_state)
          : data{accept_state}
          , parsingAction{ParsingAction::ACCEPT}
        {}

        [[nodiscard]] auto operator==(const Action &other) const noexcept -> bool = default;
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

        [[nodiscard]] auto getParsingAction() const noexcept -> ParsingAction
        {
            return parsingAction;
        }

        [[nodiscard]] auto getStoredData() const noexcept CCL_LIFETIMEBOUND
            -> const std::variant<std::monostate, State, LrItem> &
        {
            return data;
        }

        [[nodiscard]] auto getShiftingState() const noexcept -> State
        {
            return std::get<State>(data);
        }

        [[nodiscard]] auto getReducingItem() const noexcept CCL_LIFETIMEBOUND -> const LrItem &
        {
            return std::get<LrItem>(data);
        }
    };

    struct ActionPrintWrapper
    {
        const Action &action;
        std::function<std::string(SmallId)> idToStr;
    };
}// namespace ccl::parser

template<>
class fmt::formatter<ccl::parser::ActionPrintWrapper> : public fmt::formatter<std::string_view>
{
private:
    template<class... Ts>
    struct overloaded : Ts...
    {
        using Ts::operator()...;
    };

public:
    template<typename FmtContext>
    constexpr auto
        format(const ccl::parser::ActionPrintWrapper &action_print_wrapper, FmtContext &ctx) const
        -> decltype(auto)
    {
        using namespace ccl::parser;

        auto &action = action_print_wrapper.action;
        fmt::format_to(ctx.out(), "{} ", action.getParsingAction());

        return std::visit(
            overloaded{
                [&ctx, &action_print_wrapper](const LrItem &arg) {
                    return fmt::format_to(
                        ctx.out(), "{}", LrItemPrintWrapper(arg, action_print_wrapper.idToStr));
                },
                [&ctx](auto arg) {
                    return fmt::format_to(ctx.out(), "{}", arg);
                },
            },
            action.getStoredData());
    }
};

#endif /* CCL_PROJECT_ACTION_HPP */

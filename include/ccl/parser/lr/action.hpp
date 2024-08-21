#ifndef CCL_PROJECT_ACTION_HPP
#define CCL_PROJECT_ACTION_HPP

#include <ccl/parser/lr_item.hpp>
#include <ccl/parser/lr/detail/parsing_action.hpp>

namespace ccl::parser {
    class Action {
    private:
        std::variant<std::monostate, State, LrItem> data;
        ParsingAction parsingAction;

    public:
        explicit Action(State state)
            : data{state}
              , parsingAction{ParsingAction::SHIFT} {
        }

        explicit Action(const LrItem&item)
            : data{item}
              , parsingAction{ParsingAction::REDUCE} {
        }

        explicit Action(std::monostate accept_state)
            : data{accept_state}
              , parsingAction{ParsingAction::ACCEPT} {
        }

        [[nodiscard]] auto
        operator<=>(const Action&other) const noexcept -> std::weak_ordering = default;

        [[nodiscard]] auto isShift() const noexcept -> bool {
            return parsingAction == ParsingAction::SHIFT;
        }

        [[nodiscard]] auto isReduce() const noexcept -> bool {
            return parsingAction == ParsingAction::REDUCE;
        }

        [[nodiscard]] auto isAccept() const noexcept -> bool {
            return parsingAction == ParsingAction::ACCEPT;
        }

        [[nodiscard]] auto getParsingAction() const noexcept -> ParsingAction {
            return parsingAction;
        }

        [[nodiscard]] auto getStoredData() const noexcept CCL_LIFETIMEBOUND
            -> const std::variant<std::monostate, State, LrItem>& {
            return data;
        }

        [[nodiscard]] auto getShiftingState() const -> State {
            return std::get<State>(data);
        }

        [[nodiscard]] auto getReducingItem() const CCL_LIFETIMEBOUND -> const LrItem& {
            return std::get<LrItem>(data);
        }
    };

    struct ActionPrintWrapper {
        const Action&action;
        std::function<std::string(SmallId)> idToStr;
    };
} // namespace ccl::parser

template<>
struct fmt::formatter<ccl::parser::ActionPrintWrapper> : formatter<std::string_view> {
    auto format(const ccl::parser::ActionPrintWrapper&action_print_wrapper, format_context&ctx)
    const -> format_context::iterator;
};

#endif /* CCL_PROJECT_ACTION_HPP */

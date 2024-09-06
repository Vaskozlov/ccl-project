#ifndef CCL_PROJECT_ACTION_HPP
#define CCL_PROJECT_ACTION_HPP

#include <ccl/parser/lr/detail/parsing_action.hpp>
#include <ccl/parser/lr/lr_item.hpp>

namespace ccl::parser
{
    class Action
    {
    private:
        State shiftingState{};
        Symbol productionType{};
        SmallId numberOfElementsInProduction{};
        ParsingAction parsingAction;

    public:
        Action()
          : parsingAction{ParsingAction::ACCEPT}
        {}

        explicit Action(const State shifting_state) noexcept
          : shiftingState{shifting_state}
          , parsingAction{ParsingAction::SHIFT}
        {}

        explicit Action(
            const Symbol production_type,
            const SmallId number_of_elements_in_production) noexcept
          : productionType{production_type}
          , numberOfElementsInProduction{number_of_elements_in_production}
          , parsingAction{ParsingAction::REDUCE}
        {}

        CCL_DECL auto
            operator<=>(const Action &other) const noexcept -> std::weak_ordering = default;

        CCL_DECL auto getActionType() const noexcept -> ParsingAction
        {
            return parsingAction;
        }

        CCL_DECL auto isShift() const noexcept -> bool
        {
            return parsingAction == ParsingAction::SHIFT;
        }

        CCL_DECL auto isReduce() const noexcept -> bool
        {
            return parsingAction == ParsingAction::REDUCE;
        }

        CCL_DECL auto isAccept() const noexcept -> bool
        {
            return parsingAction == ParsingAction::ACCEPT;
        }


        CCL_DECL auto getShiftingState() const -> State
        {
            if (!isShift()) {
                throw std::bad_variant_access{};
            }

            return shiftingState;
        }

        CCL_DECL auto getProductionType() const -> Symbol
        {
            if (!isReduce()) {
                throw std::bad_variant_access{};
            }

            return productionType;
        }

        CCL_DECL auto getNumberOfElementsInProduction() const -> SmallId
        {
            if (!isReduce()) {
                throw std::bad_variant_access{};
            }

            return numberOfElementsInProduction;
        }
    };

    struct ActionPrintWrapper
    {
        std::function<std::string(SmallId)> idToStringConverter;
        Action action;
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

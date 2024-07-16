#include "ccl/parser/lr/deatil/glr_runner.hpp"

namespace ccl::parser::detail
{
    auto constructRunnerPoller(Runner runner) -> isl::Generator<ParsingAction>
    {
        using enum ccl::parser::ParsingAction;

        while (true) {
            const auto state = runner.stateStack.top();
            const auto entry = TableEntry{
                .state = state,
                .lookAhead = runner.common.word->getId(),
            };

            if (!runner.common.actionTable.contains(entry)) {
                CCL_REPEAT_FOREVER(co_yield FAILED)
            }

            const auto &actions = runner.common.actionTable.at(entry);
            runner.hostNewRunnersIfMoreThanOneAction(actions);

            const auto &current_runner_action = actions.front();
            runner.runAction(current_runner_action);

            if (current_runner_action.isAccept()) {
                CCL_REPEAT_FOREVER(co_yield ACCEPT)
            }

            if (current_runner_action.isReduce()) {
                continue;
            }

            co_yield SHIFT;
        }
    }

    auto Runner::hostNewRunnersIfMoreThanOneAction(const isl::Vector<Action> &actions) -> void
    {
        for (const auto &action : actions | std::views::drop(1)) {
            auto new_runner = *this;
            new_runner.runAction(action);
            common.pendingAlternatives.emplace_back(
                action.getParsingAction(), constructRunnerPoller(std::move(new_runner)));
        }
    }

    auto Runner::runAction(const Action &action) -> void
    {
        using enum ccl::parser::ParsingAction;

        switch (action.getParsingAction()) {
        case SHIFT:
            nodesStack.emplace(isl::makeShared<ast::TokenNode>(common.word->getId(), *common.word));
            stateStack.emplace(action.getShiftingState());
            break;

        case REDUCE:
            reduceAction(action);
            break;

        case ACCEPT:
            common.acceptedNodes.emplace_back(nodesStack.top());
            break;

        default:
            isl::unreachable();
        }
    }

    auto Runner::reduceAction(const Action &action) -> void
    {
        const auto &lr_item = action.getReducingItem();
        auto reduced_item =
            isl::makeUnique<ast::ShNodeSequence>(isl::as<Symbol>(lr_item.getProductionType()));
        const auto number_of_elements_to_take_from_stack = lr_item.length();

        for (std::size_t i = 0; i != number_of_elements_to_take_from_stack; ++i) {
            reduced_item->addNode(nodesStack.top());
            nodesStack.pop();
            stateStack.pop();
        }

        reduced_item->reverse();
        nodesStack.emplace(std::move(reduced_item));
        stateStack.emplace(common.gotoTable.at({
            stateStack.top(),
            lr_item.getProductionType(),
        }));
    }
}// namespace ccl::parser::detail

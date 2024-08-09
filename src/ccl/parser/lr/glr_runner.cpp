#include "ccl/parser/lr/detail/glr_runner.hpp"
#include "ccl/parser/ast/node_sequence.hpp"

namespace ccl::parser::detail
{
    using enum ccl::parser::ParsingAction;

    auto Runner::poll() -> ParsingAction
    {
        const auto new_state = stateStack.top();

        const auto entry = TableEntry{
            .state = new_state,
            .lookAhead = common->word->getToken().getId(),
        };

        if (!common->actionTable.contains(entry)) {
            return FAILED;
        }

        const auto &actions = common->actionTable.at(entry);
        hostNewRunnersIfMoreThanOneAction(actions);

        const auto &current_runner_action = actions.front();
        runAction(current_runner_action);

        if (current_runner_action.isAccept()) {
            return ACCEPT;
        }

        if (current_runner_action.isReduce()) {
            return poll();
        }

        return SHIFT;
    }

    auto Runner::hostNewRunnersIfMoreThanOneAction(const std::vector<Action> &actions) -> void
    {
        for (const auto &action : actions | std::views::drop(1)) {
            auto new_runner = *this;
            new_runner.runAction(action);

            switch (action.getParsingAction()) {
            case SHIFT:
                common->newRunnersInShiftState.emplace_front(std::move(new_runner));
                break;

            case REDUCE:
                common->newRunnersInReduceState.emplace_front(std::move(new_runner));
                break;

            case ACCEPT:
                common->acceptedNodes.emplace_back(new_runner.nodesStack.top());
                break;

            case FAILED:
                break;

            default:
                isl::unreachable();
            }
        }
    }

    auto Runner::runAction(const Action &action) -> void
    {
        switch (action.getParsingAction()) {
        case SHIFT:
            nodesStack.emplace(common->word);
            stateStack.emplace(action.getShiftingState());
            break;

        case REDUCE:
            reduceAction(action);
            break;

        case ACCEPT:
            common->acceptedNodes.emplace_back(nodesStack.top());
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
        const auto number_of_elements_to_take_from_stack = lr_item.size();

        for (std::size_t i = 0; i != number_of_elements_to_take_from_stack; ++i) {
            reduced_item->addNode(nodesStack.top());
            nodesStack.pop();
            stateStack.pop();
        }

        reduced_item->reverse();

        nodesStack.emplace(std::move(reduced_item));
        stateStack.emplace(common->gotoTable.at({
            stateStack.top(),
            lr_item.getProductionType(),
        }));
    }
}// namespace ccl::parser::detail

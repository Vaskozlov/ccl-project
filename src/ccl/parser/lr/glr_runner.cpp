#include <ccl/parser/dot/dot_repr.hpp>
#include <ccl/parser/lr/detail/glr_runner.hpp>
#include <isl/thread/id_generator.hpp>

namespace ccl::parser::detail
{
    using enum ParsingAction;

    static constexpr auto debugOutputEnabled = true;
    isl::thread::IdGenerator<SmallId> GlrRunner::runnerIdGenerator{0};

    namespace debug
    {
        auto stateOutput(GlrRunner &runner) -> void
        {
            if constexpr (debugOutputEnabled) {
                static auto state_counter = isl::thread::IdGenerator<SmallId>{0};
                static auto dir_created = std::atomic_flag();
                auto output_dir = std::filesystem::current_path().append("trace");

                if (!dir_created.test_and_set()) {
                    std::filesystem::create_directories(output_dir);
                }

                auto converted = isl::dot::createDotRepresentation<ast::Node *>(
                    runner.nodeStack, [&runner](const ast::Node* node) {
                        return runner.common->idToStringConverter(node->getType());
                    });

                isl::io::writeToFile(
                    output_dir.append(
                        fmt::format("trace-{}-{}.dot", runner.runnerId, state_counter.next())),
                    converted);
            }
        }
    }// namespace debug

    auto GlrRunner::poll() -> ParsingAction
    {
        auto has_reduced = false;

        while (true) {
            const auto entry = TableEntry{
                .state = stateStack.top(),
                .symbol = common->word->getType(),
            };

            auto possible_actions_it = common->actionTable.find(entry);

            if (possible_actions_it == common->actionTable.end()) {
                return FAILED;
            }

            const auto &possible_actions = possible_actions_it->second;
            hostRunnersForAlternatives(possible_actions);

            const auto &action_for_current_runner = possible_actions.front();
            runAction(action_for_current_runner);

            debug::stateOutput(*this);

            if (action_for_current_runner.isReduce()) {
                has_reduced = true;
                continue;
            }

            if (has_reduced && tryToJoin()) {
                return JOINED;
            }

            return action_for_current_runner.getParsingAction();
        }
    }

    auto GlrRunner::runAction(const Action &action) -> void
    {
        using enum ParsingAction;

        const auto action_type = action.getParsingAction();

        switch (action_type) {
        case SHIFT:
            shiftAction(action);
            break;

        case REDUCE:
            reduceAction(action);
            break;

        case ACCEPT:
            break;

        default:
            isl::unreachable();
        }
    }

    auto GlrRunner::shiftAction(const Action &action) -> void
    {
        stateStack.emplace(action.getShiftingState());
        nodeStack.push(inputLevel, stateStack.top(), common->word);
        ++inputLevel;
    }

    auto GlrRunner::reduceAction(const Action &action) -> void
    {
        const auto &lr_item = action.getReducingItem();
        const auto production = lr_item.production;
        const auto *rule = lr_item.getRulePtr();
        const auto number_of_elements_to_pop = lr_item.size();

        for (std::size_t i = 0; i != number_of_elements_to_pop; ++i) {
            stateStack.pop();
        }

        const auto new_state = common->gotoTable.at({
            stateStack.top(),
            production,
        });

        auto reducer = [&rule, production](std::vector<ast::Node *> nodes) {
            std::ranges::reverse(nodes);
            return rule->construct(production, std::move(nodes));
        };

        stateStack.emplace(new_state);
        nodeStack.reduce(inputLevel, new_state, number_of_elements_to_pop, reducer);
    }

    auto GlrRunner::hostRunnersForAlternatives(const std::vector<Action> &actions) -> void
    {
        using enum ParsingAction;

        for (const auto &action : actions | std::views::drop(1)) {
            auto new_runner = *this;
            new_runner.runnerId = runnerIdGenerator.next();

            const auto action_type = action.getParsingAction();
            new_runner.runAction(action);

            switch (action_type) {
            case SHIFT: {
                auto &created_runner =
                    common->newRunnersInShiftState.emplace_front(std::move(new_runner));
                common->stacks.emplace_front(std::addressof(created_runner.nodeStack));
            } break;

            case REDUCE: {
                auto &created_runner =
                    common->newRunnersInReduceState.emplace_front(std::move(new_runner));
                common->stacks.emplace_front(std::addressof(created_runner.nodeStack));
            } break;

            case ACCEPT:
                break;

            default:
                isl::unreachable();
            }
        }
    }

    auto GlrRunner::tryToJoin() -> bool
    {
        auto *self_stack_ptr = std::addressof(nodeStack);

        for (auto *stack : common->stacks) {
            if (stack->canMerge(nodeStack)) {
                stack->merge(nodeStack);
                std::erase(common->stacks, self_stack_ptr);
                return true;
            }
        }

        return false;
    }
}// namespace ccl::parser::detail

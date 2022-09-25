#include <ccl/parser/parser.hpp>
#include <fmt/ranges.h>
#include <fmt/std.h>

namespace ccl::parser
{
    auto Parser::parse() -> void
    {
        auto stack = Stack{};
        auto follow_set = FollowSet{};

        for (const auto &rule : parsing_rules.at(ParsingRuleType::ROOT)) {
            follow_set.push_back(&rule);
        }

        pushNewToken(stack);
        parse(stack, follow_set);
    }

    // NOLINTNEXTLINE(recursive function)
    auto Parser::parse(Stack &stack, const FollowSet &follow_set) -> bool
    {
        fmt::print("Stack:\n");

        for (const auto &node : stack) {
            node->print();
        }

        for (const auto &rule : follow_set) {
            auto mismatch_result = mismatch(stack, *rule);
            const bool fully_matched = fullyMatched(mismatch_result);

            if (fully_matched) {
                return fullMatchCase(stack, follow_set, *rule);
            }

            if (isNonTerminal(mismatch_result)) {
                return nonTerminalCase(stack, follow_set, mismatch_result);
            }

            if (isTerminal(mismatch_result, stack, follow_set)) {
                auto result = terminalCase(stack, follow_set, mismatch_result, *rule);

                if (result == TerminalMatchResult::CONTINUE) {
                    continue;
                }

                return static_cast<bool>(result);
            }
        }

        return false;
    }

    // NOLINTNEXTLINE (recursive function)
    auto Parser::terminalCase(
        Stack &stack, const FollowSet &follow_set, const MismatchResult &mismatch_result,
        const ParsingRule &rule) -> TerminalMatchResult
    {
        if (not mismatch_result.stack_version.has_value()) {
            pushNewToken(stack);
            return static_cast<size_t>(parse(stack, follow_set));
        }

        const auto &ids_to_construct = rule.ids_to_construct;
        auto target_type = ids_to_construct.front();// check THIS!

        if (follow_set.front()->type == target_type) {
            return TerminalMatchResult::CONTINUE;
        }

        auto pred = [ids_to_construct](const ParsingRule &future_rule) {
            return future_rule.ids_to_construct.front() == ids_to_construct.front();
        };

        parseWithNewFollowSet(ids_to_construct.front(), stack, true, pred);

        return static_cast<size_t>(parse(stack, follow_set));
    }

    // NOLINTNEXTLINE (recursive function)
    auto Parser::nonTerminalCase(
        Stack &stack, const FollowSet &follow_set, const MismatchResult &mismatch_result) -> bool
    {
        if (not mismatch_result.stack_version.has_value()) {
            pushNewToken(stack);
        }

        // NOLINTNEXTLINE access is checked by calle function
        parseWithNewFollowSet(*mismatch_result.rule_version, stack);
        return parse(stack, follow_set);
    }

    // NOLINTNEXTLINE (recursive function)
    auto Parser::fullMatchCase(Stack &stack, const FollowSet &follow_set, const ParsingRule &rule)
        -> bool
    {
        const auto &future_token = tokenizer.futureToken();
        auto future_token_id = future_token.getId();

        if (rule.canNotBeConstructed(future_token_id)) {
            const auto &ids_to_construct = rule.ids_to_construct;
            auto pred = [ids_to_construct](const ParsingRule &future_rule) {
                return future_rule.ids_to_construct.front() == ids_to_construct.back();
            };

            parseWithNewFollowSet(ids_to_construct.back(), stack, false, pred);
            return parse(stack, follow_set);
        }

        reduce(stack, rule);
        return true;
    }

    auto Parser::parseWithNewFollowSet(// NOLINT
        RuleId expected_type, Stack &stack, bool pass_all,
        const std::function<bool(const ParsingRule &)> &pred) -> bool
    {
        auto new_stack = Stack{};
        auto follow_set = formFollowSet(expected_type, pred);

        if (pass_all) {
            return parse(stack, follow_set);
        }

        const auto &last_elem = stack.back();
        new_stack.push_back(last_elem->clone());

        if (parse(new_stack, follow_set)) {
            stack.pop_back();
            std::ranges::move(new_stack, std::back_inserter(stack));

            return true;
        }

        return false;
    }

    [[nodiscard]] auto Parser::formFollowSet(
        RuleId expected_type, const std::function<bool(const ParsingRule &)> &pred) const
        -> FollowSet
    {
        auto follow_set = FollowSet{};

        for (auto &&rule : parsing_rules.at(expected_type)) {
            if (pred(rule)) {
                follow_set.push_back(&rule);
            }
        }

        return follow_set;
    }

    auto Parser::fullyMatched(const MismatchResult &mismatch_result) -> bool
    {
        return not(
            mismatch_result.rule_version.has_value() || mismatch_result.stack_version.has_value());
    }

    auto Parser::isTerminal(
        const MismatchResult &mismatch_result, const Stack &stack, const FollowSet &follow_set)
        -> bool
    {
        auto rule_version = mismatch_result.rule_version;

        if (not(rule_version.has_value() && terminals.contains(*rule_version))) {
            return false;
        }

        auto stack_size = stack.size();

        const auto &future_token = tokenizer.futureToken();
        auto future_id = future_token.getId();

        for (const auto &rule : follow_set) {
            const auto &ids_to_construct = rule->ids_to_construct;

            if (stack_size < rule->ids_to_construct.size() &&
                ids_to_construct[stack_size] == future_id) {
                return true;
            }
        }

        return true;
    }

    CCL_INLINE auto Parser::isNonTerminal(const MismatchResult &mismatch_result) const -> bool
    {
        return mismatch_result.rule_version.has_value() &&
               non_terminals.contains(*mismatch_result.rule_version);
    }

    CCL_INLINE auto Parser::mismatch(const Stack &stack, const ParsingRule &rule) -> MismatchResult
    {
        auto mismatch_result =
            std::ranges::mismatch(stack, rule.ids_to_construct, [](const NodePtr &node, RuleId id) {
                return id == node->getId();
            });

        auto result = MismatchResult{};

        if (mismatch_result.in1 != stack.end()) {
            result.stack_version = (*mismatch_result.in1)->getId();
        }

        if (mismatch_result.in2 != rule.ids_to_construct.end()) {
            result.rule_version = *mismatch_result.in2;
        }

        return result;
    }

    CCL_INLINE auto Parser::reduce(Stack &stack, const ParsingRule &rule) -> void
    {
        stack.push_back(rule.rule_constructor(ParsingStack(stack)));
    }

    CCL_INLINE auto Parser::pushNewToken(Stack &stack) -> void
    {
        stack.push_back(makeUnique<Node, TokenNode>(tokenizer.yield()));
    }
}// namespace ccl::parser

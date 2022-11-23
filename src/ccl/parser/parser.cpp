#include <ccl/parser/parser.hpp>
#include <fmt/ranges.h>
#include <fmt/std.h>
#include <ranges>

namespace ccl::parser
{
    [[maybe_unused]] static auto printStack(const Stack &stack) -> void
    {
        fmt::print("Stack:\n");

        for (const auto &node : stack) {
            node->print();
        }
    }

    [[maybe_unused]] static auto printFollowSet(const typename Parser::FollowSet &follow_set)
        -> void
    {
        fmt::print("Follow set: [");

        for (const ParsingRule *node : follow_set) {
            fmt::print("{}, ", node->getType());
        }

        fmt::print("]\n");
    }

    Parser::Parser(const ParsingRules &parsing_rules_, Tokenizer &tokenizer_)
      : tokenizer{tokenizer_}
      , terminals{parsing_rules_.terminals}
      , nonTerminals{parsing_rules_.non_terminals}
      , parsingRules{parsing_rules_.parsing_rules}
    {}

    auto Parser::parse() -> void
    {
        auto stack = Stack{};
        auto follow_set = FollowSet{};

        for (const auto &rule : parsingRules.at(ParsingRuleType::ROOT)) {
            follow_set.push_back(&rule);
        }

        pushNewToken(stack);
        parse(stack, follow_set);
    }

    // NOLINTNEXTLINE(recursive function)
    auto Parser::parse(Stack &stack, const FollowSet &follow_set) -> bool
    {
        printStack(stack);
        printFollowSet(follow_set);

        for (const auto &rule : follow_set) {
            auto mismatch_result = mismatch(stack, *rule);

            if (fullyMatched(mismatch_result)) {
                return fullMatchCase(stack, follow_set, *rule);
            }

            if (isNonTerminal(mismatch_result)) {
                return nonTerminalCase(stack, follow_set, mismatch_result);
            }

            if (isTerminal(mismatch_result)) {
                auto result = terminalCase(stack, follow_set, mismatch_result, *rule);

                if (TerminalMatchResult::CONTINUE == result) {
                    continue;
                }

                return as<bool>(result);
            }
        }

        throwError(stack, follow_set);
        return false;
    }

    // NOLINTNEXTLINE (recursive function)
    auto Parser::terminalCase(
        Stack &stack, const FollowSet &follow_set, const MismatchResult &mismatch_result,
        const ParsingRule &rule) -> TerminalMatchResult
    {
        if (!mismatch_result.stack_version.has_value()) {
            pushNewToken(stack);
            return as<Id>(parse(stack, follow_set));
        }

        const auto &ids_to_construct = rule.idsToConstruct;

        auto mis =
            std::ranges::mismatch(stack, ids_to_construct, [](const UniquePtr<Node> &node, Id id) {
                return id == node->getId();
            });

        auto target_type = 0ZU;

        if (mis.in2 != ids_to_construct.end()) {
            target_type = *(mis.in2 - 1);
        }

        if (follow_set.front()->type == target_type) {
            return TerminalMatchResult::CONTINUE;
        }

        auto pred = [target_type](const ParsingRule &future_rule) {
            return future_rule.idsToConstruct.front() == target_type;
        };

        parseWithNewFollowSet(target_type, stack, 2, pred);
        return as<size_t>(parse(stack, follow_set));
    }

    // NOLINTNEXTLINE (recursive function)
    auto Parser::nonTerminalCase(
        Stack &stack, const FollowSet &follow_set, const MismatchResult &mismatch_result) -> bool
    {
        if (!mismatch_result.stack_version.has_value()) {
            pushNewToken(stack);
        }

        // NOLINTNEXTLINE access is checked by calle function
        parseWithNewFollowSet(*mismatch_result.rule_version, stack, 1);
        return parse(stack, follow_set);
    }

    // NOLINTNEXTLINE (recursive function)
    auto Parser::fullMatchCase(Stack &stack, const FollowSet &follow_set, const ParsingRule &rule)
        -> bool
    {
        const auto &future_token = tokenizer.yieldFutureToken();
        auto future_token_id = future_token.getId();

        if (rule.canNotBeConstructed(future_token_id)) {
            const auto &ids_to_construct = rule.idsToConstruct;
            auto pred = [&ids_to_construct](const ParsingRule &future_rule) {
                return future_rule.idsToConstruct.front() == ids_to_construct.back();
            };

            parseWithNewFollowSet(ids_to_construct.back(), stack, 1, pred);
            return parse(stack, follow_set);
        }

        reduce(stack, rule);
        return true;
    }

    auto Parser::parseWithNewFollowSet(// NOLINT
        Id expected_type, Stack &stack, size_t passing_elements,
        const std::function<bool(const ParsingRule &)> &pred) -> bool
    {
        auto new_stack = Stack{};
        auto follow_set = formFollowSet(expected_type, pred);

        if (follow_set.empty()) {
            return false;
        }

        if (passing_elements == std::numeric_limits<size_t>::max()) {
            return parse(stack, follow_set);
        }

        for (auto i = 0ZU; i != passing_elements; ++i) {
            new_stack.insert(new_stack.begin(), std::move(stack.back()));
            stack.pop_back();
        }

        fmt::print("new stack: \n");
        for (const auto &node : new_stack) {
            node->print();
        }

        if (parse(new_stack, follow_set)) {
            std::ranges::move(new_stack, std::back_inserter(stack));
            return true;
        }

        return false;
    }

    auto Parser::formFollowSet(
        Id expected_type, const std::function<bool(const ParsingRule &)> &pred) const -> FollowSet
    {
        auto follow_set = FollowSet{};

        if (!parsingRules.contains(expected_type)) {
            return {};
        }

        for (auto &&rule : parsingRules.at(expected_type) | std::views::filter(pred)) {
            follow_set.push_back(&rule);
        }

        return follow_set;
    }

    auto Parser::fullyMatched(const MismatchResult &mismatch_result) -> bool
    {
        return not(
            mismatch_result.rule_version.has_value() || mismatch_result.stack_version.has_value());
    }

    auto Parser::isTerminal(const MismatchResult &mismatch_result) -> bool
    {
        auto rule_version = mismatch_result.rule_version;
        return rule_version.has_value() && terminals.contains(*rule_version);
    }

    CCL_INLINE auto Parser::isNonTerminal(const MismatchResult &mismatch_result) const -> bool
    {
        return mismatch_result.rule_version.has_value() &&
               nonTerminals.contains(*mismatch_result.rule_version) &&
               not(mismatch_result.stack_version.has_value() &&
                   terminals.contains(*mismatch_result.stack_version));
    }

    CCL_INLINE auto Parser::mismatch(const Stack &stack, const ParsingRule &rule) -> MismatchResult
    {
        auto mismatch_result = std::ranges::mismatch(
            stack, rule.idsToConstruct,
            [](const UniquePtr<Node> &node, Id id) { return id == node->getId(); });

        auto result = MismatchResult{};

        if (mismatch_result.in1 != stack.end()) {
            result.stack_version = (*mismatch_result.in1)->getId();
        }

        if (mismatch_result.in2 != rule.idsToConstruct.end()) {
            result.rule_version = *mismatch_result.in2;
        }

        return result;
    }

    CCL_INLINE auto Parser::reduce(Stack &stack, const ParsingRule &rule) -> void
    {
        stack.push_back(rule.constructor(ParsingStack(stack)));
    }

    CCL_INLINE auto Parser::pushNewToken(Stack &stack) -> void
    {
        stack.push_back(makeUnique<Node, TokenNode>(tokenizer.yield()));
    }

    auto Parser::throwError(Stack &stack, const FollowSet &follow_set) -> void
    {
        if (stack.empty()) {
            return;
        }

        auto last_elem = std::move(stack.back());
        stack.pop_back();

        auto *as_token_node = as<TokenNode *>(last_elem.get());

        if (as_token_node == nullptr) {
            throw UnrecoverableError{"last stack element bust be a token!"};
        }

        const auto &token = as_token_node->getToken();
        auto expected_types = std::vector<std::string>();

        std::ranges::transform(
            follow_set, std::back_inserter(expected_types),
            [](const auto *rule) { return std::string(rule->getName()); });

        auto message = fmt::format(
            "Unexpected token! Any rule from [{}] can not be constructed using {}",
            fmt::join(expected_types, ", "), token.getRepr());

        auto exception = text::TextIteratorException(
            ExceptionCriticality::CRITICAL, AnalysationStage::PARSING, token.getLocation(),
            token.getReprSize(), token.getWorkingLine(), message);

        exception_handler.handle(exception);
    }
}// namespace ccl::parser

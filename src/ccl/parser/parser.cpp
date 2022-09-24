#include <ccl/parser/parser.hpp>
#include <fmt/ranges.h>
#include <fmt/std.h>

namespace ccl::parser
{
    std::atomic<size_t> ParsingRule::uuid_counter{ 0 };// NOLINT (global variable)

    ParsingRules::ParsingRules(const std::initializer_list<ParsingRule> &rules_)
    {
        initializeRules(rules_);
    }

    auto ParsingRules::initializeRules(const std::initializer_list<ParsingRule> &initializer_list)
        -> void
    {
        for (const auto &rule : initializer_list) {
            rules[rule.type].push_back(rule);
            rules[rule.type].back().uuid = std::atomic_fetch_add_explicit(
                &ParsingRule::uuid_counter, 1UL, std::memory_order_relaxed);

            fmt::print("{}: {}\n", rule.name, rule.ids_to_constructs);
        }
    }

    auto Parser::parse() -> void
    {
        auto follow_set =
            Flatmap{ { ParsingRuleType::ROOT, parsing_rules.at(ParsingRuleType::ROOT) } };

        parse(follow_set);
    }

    // NOLINTNEXTLINE
    auto Parser::parse(const Flatmap &follow_set) -> bool
    {
        if (stack.empty()) {
            pushNewToken();
        }

        fmt::print("follow set: {}\nStack:\n", follow_set);

        for (const auto &elem : stack) {
            elem->print();
        }

        fmt::print("----------------------------------------------------------------\n\n");

        for (const auto &[rule_type, rules] : follow_set) {
            for (const auto &rule : rules) {
                switch (isRuleOnStack(rule)) {
                case RuleOnStackResult::NO_MATCH:
                    if (not parseWithNewFollowSet(follow_set, rule)) {
                        continue;
                    }
                    parse(follow_set);
                    break;

                case RuleOnStackResult::PARTIAL_MATCH:
                    fmt::print("PARTIAL_MATCH {}: {}\n", rule.name, rule.ids_to_constructs);
                    pushNewToken();
                    parse(follow_set);
                    break;

                case RuleOnStackResult::FULL_MATCH:
                    fmt::print("FULL MATCH\n");
                    reduce(rule);
                    return true;
                }
            }
        }

        return false;
    }

    // NOLINTNEXTLINE
    auto Parser::parseWithNewFollowSet(const Flatmap &follow_set, const ParsingRule &rule)
        -> bool// NOLINT
    {
        auto new_follow_set = Flatmap{};

        for (const auto &[rule_type, rules] : follow_set) {
            for (const auto &follow_rule : rules) {
                auto next_possible_id = follow_rule.ids_to_constructs.front();

                if (next_possible_id != rule.type && parsing_rules.contains(next_possible_id)) {
                    new_follow_set[next_possible_id] = parsing_rules.at(next_possible_id);
                }
            }
        }

        return parse(new_follow_set);
    }

    auto Parser::isRuleOnStack(const ParsingRule &rule) const -> RuleOnStackResult
    {
        const auto &future_token = tokenizer.futureToken();

        if (rule.ids_that_forbid_construction.contains(future_token.getId())) {
            return RuleOnStackResult::PRECEDENCE_FORBIDION;
        }

        const auto &ids_to_construct = rule.ids_to_constructs;

        auto ids_to_construct_length = static_cast<ssize_t>(ids_to_construct.size());

        auto stack_begin = std::max(stack.begin(), stack.end() - ids_to_construct_length);

        auto mismatch = std::ranges::mismatch(
            ids_to_construct.rbegin(), ids_to_construct.rend(), stack_begin, stack.end(),
            [](size_t id, const NodePtr &node) { return id == node->getId(); });

        if (mismatch.in1 == ids_to_construct.rend()) {
            return RuleOnStackResult::FULL_MATCH;
        }

        if (mismatch.in1 != ids_to_construct.rbegin() && ids_to_construct.size() > stack.size()) {
            return RuleOnStackResult::PARTIAL_MATCH;
        }

        return RuleOnStackResult::NO_MATCH;
    }

    auto Parser::reduce(const ParsingRule &rule) -> void
    {
        stack.push_back(rule.rule_constructor(ParsingStack(stack)));
    }

    auto Parser::pushNewToken() -> void
    {
        stack.push_back(std::make_unique<TokenNode>(tokenizer.yield()));
    }
}// namespace ccl::parser

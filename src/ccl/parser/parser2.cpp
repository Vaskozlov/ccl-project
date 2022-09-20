#include <ccl/parser/parser2.hpp>
#include <ranges>

namespace ccl::parser
{
    ParsingRules2::ParsingRules2(
        const std::initializer_list<RuleForParsing> &rules_,
        PrecedenceTable precedence_table_,
        std::string_view (*token_to_string_)(size_t))
      : precedence_table(std::move(precedence_table_)), token_formatter(token_to_string_)
    {
        initializeTable(rules_);
        fixConflicts();
    }

    auto ParsingRules2::initializeTable(const std::initializer_list<RuleForParsing> &rules) -> void
    {
        size_t identifier_factory = 0;

        for (const auto &rule : rules) {
            if (rule.ids_to_construct.empty()) {
                continue;
            }

            auto first_id = rule.ids_to_construct.at(0);
            first_set[first_id].push_back(rule);
            first_set[first_id].back().id = identifier_factory++;
        }
    }

    auto ParsingRules2::fixConflicts() -> void
    {
        for (auto &[id, rules] : first_set) {
            for (auto &rule : rules) {
                fmt::print(
                    "{} {}\n", token_formatter.formatContainerOfTokens(rule.ids_to_construct),
                    token_formatter.formatContainerOfTokens(rule.forbidden_future_ids));

                fixShiftReduceConflict(rule);
                fixReduceReduceConflict(rule);

                fmt::print(
                    "{} {}\n\n", token_formatter.formatContainerOfTokens(rule.ids_to_construct),
                    token_formatter.formatContainerOfTokens(rule.forbidden_future_ids));
            }
        }
    }

    auto ParsingRules2::fixShiftReduceConflict(RuleForParsing &rule) -> void
    {
        auto &ids_to_construct = rule.ids_to_construct;
        auto first_id = ids_to_construct.front();
        auto &rules_with_similar_begin = first_set[first_id];

        for (auto &similar_rule : rules_with_similar_begin) {
            if (ids_to_construct == similar_rule.ids_to_construct) {
                continue;
            }

            fixShiftReduceConflict(rule, similar_rule);
        }
    }

    auto ParsingRules2::fixShiftReduceConflict(RuleForParsing &rule, RuleForParsing &similar_rule)
        -> void
    {
        auto mismatch = std::ranges::mismatch(rule.ids_to_construct, similar_rule.ids_to_construct);

        if (mismatch.in1 == rule.ids_to_construct.end()) {
            rule.forbidden_future_ids.insert(*mismatch.in2);
        }
    }

    auto ParsingRules2::fixReduceReduceConflict(RuleForParsing &rule) -> void
    {
        auto &ids_to_construct = rule.ids_to_construct;
        auto last_id = ids_to_construct.back();
        auto &rules_with_begin_equal_to_rule_end = first_set[last_id];

        for (auto &conflicted_rule : rules_with_begin_equal_to_rule_end) {
            if (ids_to_construct == conflicted_rule.ids_to_construct ||
                (not rule.forbidden_future_ids.empty())) {// is it enough to solve the problem?
                continue;
            }

            fixReduceReduceConflict(rule, conflicted_rule);
        }
    }

    auto ParsingRules2::fixReduceReduceConflict(
        RuleForParsing &rule, RuleForParsing &conflicted_rule) -> void
    {
        auto mismatch = std::ranges::mismatch(
            rule.ids_to_construct.rbegin(), rule.ids_to_construct.rend(),
            conflicted_rule.ids_to_construct.rbegin(), conflicted_rule.ids_to_construct.rend());

        assert(
            mismatch.in1 != rule.ids_to_construct.rend() &&
            mismatch.in2 != conflicted_rule.ids_to_construct.rend());

        auto id1 = *mismatch.in1;
        auto id2 = *mismatch.in2;

        if (precedence_table.contains(id1) && precedence_table.contains(id2)) {
            auto precedence1 = precedence_table.at(id1);
            auto precedence2 = precedence_table.at(id2);

            if (precedence1 < precedence2) {
                rule.forbidden_future_ids.insert(id2);
            }
        } else {
            fmt::print(
                "Unable to solve reduce/reduce conflict, because there are not enough information "
                "in the table: {}({}) {}({})\n",
                token_formatter.formatContainerOfTokens(rule.ids_to_construct),
                token_formatter.formatToken(id1),
                token_formatter.formatContainerOfTokens(conflicted_rule.ids_to_construct),
                token_formatter.formatToken(id2));
        }
    }

    auto Parser2::parser() -> void
    {
        auto &token = tokenizer.yield();

        while (token.getId() != 0) {
            stack.push_back(std::make_unique<TokenNode>(token));

            iteration();

            token = tokenizer.yield();
        }

        stack.front()->print();
    }

    // NOLINTNEXTLINE(recursion)
    auto Parser2::iteration() -> void
    {
        auto &first_stack_elem = stack.back();
        auto first_id = first_stack_elem->getId();

        if (not first_set.contains(first_id)) {
            return;// TODO: check follow set
        }

        const auto &rules = first_set.at(first_id);

        fmt::print("{}\n", token_formatter.formatContainerOfTokens(stack, [](const NodePtr &ptr) {
            return ptr->getId();
        }));

        for (const auto &rule : rules) {
            if (applyRule(rule)) {
                return iteration();
            }
        }
    }

    auto Parser2::applyRule(const RuleForParsing &rule) -> bool
    {
        auto &next_token = tokenizer.futureToken();
        auto next_id = next_token.getId();

        if (needToSkip(rule, next_id)) {
            return false;
        }

        if (matchesRule(rule)) {
            stack.push_back(rule.rule_constructor(StackGetter2(stack)));
            return true;
        }

        return false;
    }

    auto Parser2::matchesRule(const RuleForParsing &rule) -> bool
    {
        auto stack_it = stack.rbegin();

        return std::ranges::all_of(
            rule.ids_to_construct, [&stack_it](size_t id) { return id == (*stack_it++)->getId(); });
    }

    auto Parser2::needToSkip(const RuleForParsing &rule, size_t next_id) const -> bool
    {
        const auto &ids_to_construct = rule.ids_to_construct;
        const auto &forbidden_future_ids = rule.forbidden_future_ids;

        return forbidden_future_ids.contains(next_id) || (stack.size() < ids_to_construct.size());
    }
}// namespace ccl::parser

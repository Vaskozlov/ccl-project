#include <ccl/parser/parsing_rules.hpp>
#include <ranges>

namespace ccl::parser
{
    [[maybe_unused]] static auto printRuleFully(const ParsingRule &rule) -> void
    {
        fmt::print(
            "{}: {} {}\n\n", rule, rule.getIdsToConstruct(), rule.getIdsThatForbidConstruction());
    }

    ParsingRules::ParsingRules(
        PrecedenceTable precedence_table_, InitializerList<ParsingRule> rules_)
      : precedence_table(std::move(precedence_table_))
    {
        precedence_table[ParsingRuleType::EOI] = 0;

        initializeRules(rules_);
        detectTerminals();
        fixConflicts();
    }

    auto ParsingRules::initializeRules(InitializerList<ParsingRule> initializer_list) -> void
    {
        for (const auto &const_rule : initializer_list) {
            auto rule = const_rule;
            rule.generateUuid();

            non_terminals.insert(rule.type);
            parsing_rules[rule.type].push_back(std::move(rule));
        }
    }

    auto ParsingRules::detectTerminals() -> void
    {
        auto inserter = std::inserter(terminals, terminals.begin());
        auto insert_condition = [this](RuleId id) { return not non_terminals.contains(id); };

        for (const auto &[keys, rules] : parsing_rules) {
            for (const auto &rule : rules) {
                const auto &ids_to_construct = rule.ids_to_construct;
                std::ranges::copy_if(ids_to_construct, inserter, insert_condition);
            }
        }
    }

    auto ParsingRules::fixConflicts() -> void
    {
        auto all_rules = Vector<ParsingRule *>{};

        for (auto &[keys, rules] : parsing_rules) {
            for (auto &rule : rules) {
                all_rules.push_back(&rule);
            }
        }

        for (auto *rule : all_rules) {
            auto check_not_on_self = [&rule](const ParsingRule *other_rule) {
                return rule->uuid != other_rule->uuid;
            };

            checkThereAreNoCloseNonTerminals(*rule);

            for (const auto *other_rule : all_rules | std::views::filter(check_not_on_self)) {
                fixConflict(*rule, *other_rule);
            }
        }
    }

    auto ParsingRules::checkThereAreNoCloseNonTerminals(const ParsingRule &rule) -> void
    {
        const auto &ids_to_construct = rule.ids_to_construct;

        for (auto i = 1ZU; i != ids_to_construct.size(); ++i) {
            if (non_terminals.contains(ids_to_construct[i - 1]) &&
                non_terminals.contains(ids_to_construct[i])) {
                throw std::logic_error(fmt::format(
                    "Two non-terminals elements can not be close to each other! In {} {} is non "
                    "terminal as well as {}",
                    ids_to_construct, ids_to_construct[i - 1], ids_to_construct[i]));
            }
        }
    }

    auto ParsingRules::fixConflict(ParsingRule &rule, const ParsingRule &other_rule) -> void
    {
        auto mismatch = std::ranges::mismatch(rule.ids_to_construct, other_rule.ids_to_construct);

        if (mismatch.in1 == rule.ids_to_construct.end()) {
            if (mismatch.in2 == rule.ids_to_construct.end()) {
                throw std::logic_error(fmt::format(
                    "Rules are identical! {} {}\n", rule.ids_to_construct,
                    other_rule.ids_to_construct));
            }

            rule.ids_that_forbid_construction.insert(*mismatch.in2);
            return;
        }

        if (mismatch.in1 != rule.ids_to_construct.begin() &&
            mismatch.in2 != other_rule.ids_to_construct.end()) {
            auto first = *mismatch.in1;
            auto second = *mismatch.in2;

            if (precedence_table.contains(first) && precedence_table.contains(second)) {
                return fixUsingPrecedence(rule, first, second);
            }

            throw std::logic_error(fmt::format(
                "Not enough precedence rules for {} {}\n", rule.ids_to_construct,
                other_rule.ids_to_construct));
        }
    }

    auto ParsingRules::fixUsingPrecedence(
        ParsingRule &rule, RuleId first_mismatch, RuleId second_mismatch) const -> void
    {
        auto first_precedence = precedence_table.at(first_mismatch);
        auto second_precedence = precedence_table.at(second_mismatch);

        if (first_precedence < second_precedence) {
            rule.ids_that_forbid_construction.insert(second_mismatch);
        }

        return;
    }
}// namespace ccl::parser

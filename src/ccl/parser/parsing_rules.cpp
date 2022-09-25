#include <ccl/parser/parsing_rules.hpp>

namespace ccl::parser
{
    static auto printRuleFully(const ParsingRule &rule) -> void
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

            fmt::print("{}: {}\n", const_rule, const_rule.ids_to_construct);
        }
    }

    auto ParsingRules::detectTerminals() -> void
    {
        auto inserter = std::inserter(terminals, terminals.begin());
        auto insert_condition = [this](RuleId id) { return not non_terminals.contains(id); };

        for (const auto &[type, rules] : parsing_rules) {
            for (const auto &rule : rules) {
                const auto &ids_to_construct = rule.ids_to_construct;
                std::ranges::copy_if(ids_to_construct, inserter, insert_condition);
            }
        }
    }

    auto ParsingRules::fixConflicts() -> void
    {
        auto rules = Vector<ParsingRule *>{};

        for (auto &[type, sub_rules] : parsing_rules) {
            for (auto &rule : sub_rules) {
                rules.push_back(&rule);
            }
        }

        for (auto *rule : rules) {
            checkThereAreNoCloseNonTerminals(*rule);

            for (auto *other_rule : rules) {
                if (rule->uuid == other_rule->uuid) {
                    continue;
                }

                printRuleFully(*rule);
                fixConflict(*rule, *other_rule);
                printRuleFully(*rule);
            }
        }
    }

    auto ParsingRules::checkThereAreNoCloseNonTerminals(const ParsingRule &rule) -> void
    {
        const auto &ids_to_construct = rule.ids_to_construct;

        for (size_t i = 1; i != ids_to_construct.size(); ++i) {
            if (non_terminals.contains(ids_to_construct[i - 1]) &&
                non_terminals.contains(ids_to_construct[i])) {
                throw std::logic_error(fmt::format(
                    "Two non-terminals elements can not be close to each other! In {} {} is non "
                    "terminal as well as {}",
                    ids_to_construct, ids_to_construct[i - 1], ids_to_construct[i]));
            }
        }
    }

    auto ParsingRules::fixConflict(ParsingRule &rule, const ParsingRule &other) -> void
    {
        auto mismatch = std::ranges::mismatch(rule.ids_to_construct, other.ids_to_construct);

        if (mismatch.in1 == rule.ids_to_construct.end()) {
            if (mismatch.in2 == rule.ids_to_construct.end()) {
                throw std::logic_error(fmt::format(
                    "Rules are identical! {} {}\n", rule.ids_to_construct, other.ids_to_construct));
            }

            rule.ids_that_forbid_construction.insert(*mismatch.in2);
            return;
        }

        if (mismatch.in1 != rule.ids_to_construct.begin() &&
            mismatch.in2 != other.ids_to_construct.end()) {
            auto first = *mismatch.in1;
            auto second = *mismatch.in2;

            if (precedence_table.contains(first) && precedence_table.contains(second)) {
                auto first_precedence = precedence_table.at(first);
                auto second_precedence = precedence_table.at(second);

                if (first_precedence < second_precedence) {
                    rule.ids_that_forbid_construction.insert(second);
                }

                return;
            }

            throw std::logic_error(fmt::format(
                "Not enough precedence rules for {} {}\n", rule.ids_to_construct,
                other.ids_to_construct));
        }
    }
}// namespace ccl::parser

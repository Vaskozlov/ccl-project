#include "ccl/parser/detail/empty_moves_fixer.hpp"
#include "ccl/parser/rules_reader/rules_lexer.hpp"

#include <ccl/parser/first_set.hpp>
#include <ccl/parser/follow_set.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>

namespace ccl::parser
{
    GrammarStorage::GrammarStorage(const Symbol epsilon)
      : grammarSymbols{0, epsilon}
      , possiblyEmptyRules{epsilon}
      , epsilonSymbol{epsilon}
    {}

    GrammarStorage::GrammarStorage(
        const bool remove_epsilon, const Symbol epsilon, const Symbol goal_production,
        const std::initializer_list<isl::Pair<Symbol, std::vector<Rule>>> &initial_data)
      : grammarSymbols{0, epsilon}
      , possiblyEmptyRules{epsilon}
      , epsilonSymbol{epsilon}
    {
        for (const auto &[key, alternatives] : initial_data) {
            for (const auto &rule : alternatives) {
                tryEmplace(key, rule);
            }
        }

        finishGrammar(goal_production, remove_epsilon);
    }

    auto GrammarStorage::tryEmplace(const Symbol key, Rule rule) -> bool
    {
        auto [it, inserted] = rules.try_emplace(key);

        if (std::ranges::find(it->second, rule) == it->second.end()) {
            it->second.emplace_back(std::move(rule));
            return true;
        }

        return false;
    }

    auto GrammarStorage::eraseRule(const Symbol key, const Rule &rule) -> void
    {
        if (const auto it = rules.find(key); it != rules.end()) {
            it->second.erase(std::ranges::find(it->second, rule));
        }
    }

    auto GrammarStorage::registerAllRuleSymbols(const Rule &rule) -> void
    {
        for (const auto symbol : rule) {
            grammarSymbols.emplace(symbol);
        }
    }

    auto GrammarStorage::getStartRule(const Symbol start_production_id) const -> const Rule *
    {
        const auto &start_rules = at(start_production_id);

        if (start_rules.size() > 1) [[unlikely]] {
            throw std::runtime_error{"Grammar must define only one goal production."};
        }

        if (start_rules.empty()) [[unlikely]] {
            throw std::runtime_error{"No goal production found."};
        }

        const auto *start_rule = std::addressof(start_rules.front());

        if (start_rule->size() != 1) [[unlikely]] {
            throw std::runtime_error{"Goal production must have one symbol."};
        }

        return start_rule;
    }

    auto GrammarStorage::finishGrammar(const Symbol goal_production, const bool remove_epsilon)
        -> void
    {
        for (const auto &[key, alternatives] : rules) {
            nonTerminals.emplace(key);
            grammarSymbols.emplace(key);

            for (const auto &rule : alternatives) {
                registerAllRuleSymbols(rule);
            }
        }

        if (remove_epsilon) {
            findAndFixEmptyRules();
        }

        auto first_set_evaluator = FirstSetEvaluator{
            epsilonSymbol,
            *this,
        };

        firstSet = std::move(first_set_evaluator.getFirstSet());

        auto follow_set_evaluator = FollowSetEvaluator{
            goal_production, 0, epsilonSymbol, *this, firstSet,
        };

        followSet = std::move(follow_set_evaluator.getFollowSet());
    }

    auto GrammarStorage::findAndFixEmptyRules() -> void
    {
        auto empty_moves_fixer = detail::EmptyMovesFixer{*this};
        empty_moves_fixer.apply();
    }
}// namespace ccl::parser
#include "ccl/parser/detail/empty_moves_fixer.hpp"
#include <ccl/parser/grammar_rules_storage.hpp>

namespace ccl::parser
{
    GrammarRulesStorage::GrammarRulesStorage(Symbol epsilon)
      : grammarSymbols{0, epsilon}
      , possiblyEmptyRules{epsilon}
      , epsilonSymbol{epsilon}
    {}

    GrammarRulesStorage::GrammarRulesStorage(
        Symbol epsilon,
        const std::initializer_list<isl::Pair<Symbol, std::vector<Rule>>> &initial_data)
      : Self{initial_data.begin(), initial_data.end()}
      , grammarSymbols{0, epsilon}
      , possiblyEmptyRules{epsilon}
      , epsilonSymbol{epsilon}
    {
        finishGrammar();
    }

    auto GrammarRulesStorage::tryEmplace(Symbol key, Rule rule) -> bool
    {
        auto [it, inserted] = Self::try_emplace(key);

        if (std::ranges::find(it->second, rule) == it->second.end()) {
            it->second.emplace_back(std::move(rule));
            return true;
        }

        return false;
    }

    auto GrammarRulesStorage::eraseRule(Symbol key, const Rule &rule) -> void
    {
        auto it = Self::find(key);

        if (it != Self::end()) {
            it->second.erase(std::ranges::find(it->second, rule));
        }
    }

    auto GrammarRulesStorage::registerAllRuleSymbols(const Rule &rule) -> void
    {
        for (const auto symbol : rule) {
            grammarSymbols.emplace(symbol);
        }
    }

    auto GrammarRulesStorage::finishGrammar() -> void
    {
        for (const auto &[key, rules] : *this) {
            nonTerminals.emplace(key);
            grammarSymbols.emplace(key);

            for (const auto &rule : rules) {
                registerAllRuleSymbols(rule);
            }
        }

        findAndFixEmptyRules();
    }

    auto GrammarRulesStorage::findAndFixEmptyRules() -> void
    {
        auto empty_moves_fixer = detail::EmptyMovesFixer{*this};
        empty_moves_fixer.apply();
    }
}// namespace ccl::parser
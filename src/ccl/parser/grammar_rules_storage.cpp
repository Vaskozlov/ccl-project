#include "ccl/parser/detail/empty_moves_fixer.hpp"
#include "ccl/parser/rules_reader/rules_lexer.hpp"
#include <ccl/parser/grammar_rules_storage.hpp>

namespace ccl::parser
{
    GrammarStorage::GrammarStorage(Symbol epsilon)
      : grammarSymbols{0, epsilon}
      , possiblyEmptyRules{epsilon}
      , epsilonSymbol{epsilon}
    {}

    GrammarStorage::GrammarStorage(
        bool remove_epsilon,
        Symbol epsilon,
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

        finishGrammar(remove_epsilon);
    }

    auto GrammarStorage::tryEmplace(Symbol key, Rule rule) -> bool
    {
        auto [it, inserted] = Self::try_emplace(key);

        if (std::ranges::find(it->second, rule) == it->second.end()) {
            it->second.emplace_back(std::move(rule));
            return true;
        }

        return false;
    }

    auto GrammarStorage::eraseRule(Symbol key, const Rule &rule) -> void
    {
        auto it = Self::find(key);

        if (it != Self::end()) {
            it->second.erase(std::ranges::find(it->second, rule));
        }
    }

    auto GrammarStorage::registerAllRuleSymbols(const Rule &rule) -> void
    {
        for (const auto symbol : rule) {
            grammarSymbols.emplace(symbol);
        }
    }

    auto GrammarStorage::finishGrammar(bool remove_epsilon) -> void
    {
        for (const auto &[key, rules] : *this) {
            nonTerminals.emplace(key);
            grammarSymbols.emplace(key);

            for (const auto &rule : rules) {
                registerAllRuleSymbols(rule);
            }
        }

        if (remove_epsilon) {
            findAndFixEmptyRules();
        }
    }

    auto GrammarStorage::findAndFixEmptyRules() -> void
    {
        auto empty_moves_fixer = detail::EmptyMovesFixer{*this};
        empty_moves_fixer.apply();
    }
}// namespace ccl::parser
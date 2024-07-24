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
        const std::initializer_list<isl::Pair<Symbol, isl::Vector<Rule>>> &initial_data)
      : isl::Map<Symbol, isl::Vector<Rule>>{initial_data}
      , grammarSymbols{0, epsilon}
      , possiblyEmptyRules{epsilon}
      , epsilonSymbol{epsilon}
    {
        finishGrammar();
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
        auto empty_moves_fixer = detail::EmptyRulesFixer{*this};
        empty_moves_fixer.apply();
    }
}// namespace ccl::parser
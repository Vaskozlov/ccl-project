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
      : std::map<Symbol, std::vector<Rule>>{initial_data.begin(), initial_data.end()}
      , grammarSymbols{0, epsilon}
      , possiblyEmptyRules{epsilon}
      , epsilonSymbol{epsilon}
    {
        finishGrammar();
    }
    /**
     *
     * @return optional of a follow set. Will be empty if follow set wasn't initialized.\n
     * see  <i>initializeFollowSetLazily(const Symbol start_symbol, const Symbol
     * end_symbol)</i>
     */
    auto GrammarRulesStorage::tryToGetInitializedFollowSet()
        const -> std::optional<std::map<Symbol, std::set<Symbol>>>
    {
        return lazilyInitializedFollowSet;
    }

    /**
     * Function will compute follow set if it wasn't initialized.
     * Otherwise just returns initialized follow set.
     *
     * @param start_symbol start symbol, usually a GOAL symbol.
     * @param end_symbol symbol at the end of input
     * @return initialized follow set.
     */
    auto GrammarRulesStorage::initializeFollowSetLazily(const Symbol start_symbol, const Symbol end_symbol)
            -> std::map<Symbol, std::set<Symbol>>
    {
        if (!lazilyInitializedFollowSet.has_value()) {
            const auto first_set = evaluateFirstSet(epsilonSymbol, *this);
            auto follow_set =
                evaluateFollowSet(start_symbol, end_symbol, epsilonSymbol, *this, first_set);

            lazilyInitializedFollowSet = follow_set;
        }

        return lazilyInitializedFollowSet.value();
    }

    auto GrammarRulesStorage::getNotFilledHandlers(const Symbol start_symbol,
        const Symbol end_symbol) -> std::set<Symbol>
    {

        auto follow_set = initializeFollowSetLazily(start_symbol, end_symbol);

        auto result_set = std::set<Symbol>();

        for (const auto non_terminal_symbol : getNonTerminals()) {
            if (follow_set.at(non_terminal_symbol).empty()) {
                result_set.emplace(non_terminal_symbol);
            }
        }

        return result_set;
    }

    /**
     * @param production production symbol
     * @return <b>if follow set is initialized,</b> this function return optional of a set of unused
     * symbols in given production: allGrammarSymbols \ followSet(production)\n <b>otherwise,</b>
     * empty optional. Use <i>initializeFollowSetLazily(const Symbol start_symbol, const Symbol
     * end_symbol)</i> to initialize follow set.
     */
    auto GrammarRulesStorage::getUnusedSymbolsForProduction(const Symbol production) const
        -> std::optional<std::set<Symbol>>
    {
        if (!lazilyInitializedFollowSet.has_value()) {
            return {};
        }

        auto result_set = std::set(getGrammarSymbols());

        for (auto used_symbol : lazilyInitializedFollowSet.value().at(production)) {
            result_set.erase(used_symbol);
        }

        return result_set;
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
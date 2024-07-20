#include "ccl/parser/follow_set.hpp"

namespace ccl::parser
{
    detail::FollowSetEvaluator::FollowSetEvaluator(
        Symbol start_symbol, Symbol end_of_input, Symbol epsilon_symbol,
        const isl::Set<Symbol> &grammar_symbols, const isl::Set<Symbol> &terminal_symbols,
        const GrammarRulesStorage &parser_rules,
        const isl::Map<Symbol, isl::Set<Symbol>> &first_set)
      : FirstAndFollowSetsCommon::
            FirstAndFollowSetsCommon{grammar_symbols, terminal_symbols, parser_rules}
      , firstSet{first_set}
      , epsilonSymbol{epsilon_symbol}
    {
        initializeFollowSet(start_symbol, end_of_input);
        computeFollowSet();
    }

    auto detail::FollowSetEvaluator::initializeFollowSet(Symbol start_symbol, Symbol end_of_input)
        -> void
    {
        for (auto symbol : allSymbolsInGrammar) {
            if (isNonTerminal(symbol)) {
                followSet.try_emplace(symbol);
            }
        }

        followSet.at(start_symbol).emplace(end_of_input);
    }

    auto detail::FollowSetEvaluator::computeFollowSet() -> void
    {
        applyFixedPointAlgorithmOnAllRules([this](Symbol key, const Rule &rule) {
            return followSetComputationIteration(key, rule);
        });
    }

    auto detail::FollowSetEvaluator::followSetComputationIteration(Symbol key, const Rule &rule)
        -> bool
    {
        auto has_modifications = false;
        auto trailer = followSet.at(key);

        for (auto elem : rule | std::views::reverse) {
            if (isTerminal(elem)) {
                trailer = firstSet.at(elem);
            } else {
                has_modifications = followSetNonTerminalCase(elem, trailer) || has_modifications;
            }
        }

        return has_modifications;
    }

    auto detail::FollowSetEvaluator::followSetNonTerminalCase(
        Symbol elem, isl::Set<Symbol> &trailer) -> bool
    {
        auto has_modifications = insertRange(followSet[elem], trailer);
        auto elem_first_set = firstSet.at(elem);

        if (elem_first_set.contains(epsilonSymbol)) {
            elem_first_set.erase(epsilonSymbol);
            insertRange(trailer, elem_first_set);
        } else {
            trailer = std::move(elem_first_set);
        }

        return has_modifications;
    }

    auto evaluateFollowSet(
        Symbol start_symbol, Symbol end_of_input, Symbol epsilon_symbol,
        const isl::Set<Symbol> &grammar_symbols, const isl::Set<Symbol> &terminals,
        const GrammarRulesStorage &rules,
        const isl::Map<Symbol, isl::Set<Symbol>> &first_set) -> isl::Map<Symbol, isl::Set<Symbol>>
    {
        auto follow_set = detail::FollowSetEvaluator(
            start_symbol, end_of_input, epsilon_symbol, grammar_symbols, terminals, rules,
            first_set);

        return std::move(follow_set.getFollowSet());
    }
}// namespace ccl::parser
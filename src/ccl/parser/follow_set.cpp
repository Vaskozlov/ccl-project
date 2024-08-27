#include "ccl/parser/follow_set.hpp"

namespace ccl::parser
{
    FollowSetEvaluator::FollowSetEvaluator(
        Symbol start_symbol, Symbol end_of_input, Symbol epsilon_symbol,
        GrammarStorage &parser_rules,
        const ankerl::unordered_dense::map<Symbol, std::unordered_set<Symbol>> &first_set)
      : FirstAndFollowSetsCommon{parser_rules}
      , firstSet{first_set}
      , epsilonSymbol{epsilon_symbol}
    {
        initializeFollowSet(start_symbol, end_of_input);
        computeFollowSet();
    }

    auto FollowSetEvaluator::initializeFollowSet(Symbol start_symbol, Symbol end_of_input) -> void
    {
        for (auto symbol : grammarRules.getGrammarSymbols()) {
            if (isNonTerminal(symbol)) {
                followSet.try_emplace(symbol);
            }
        }

        followSet.at(start_symbol).emplace(end_of_input);
    }

    auto FollowSetEvaluator::computeFollowSet() -> void
    {
        applyFixedPointAlgorithmOnAllRules([this](Symbol key, Rule &rule) {
            return followSetComputationIteration(key, rule);
        });
    }

    auto FollowSetEvaluator::followSetComputationIteration(Symbol key, Rule &rule) -> bool
    {
        auto has_modifications = false;
        auto trailer = followSet.at(key);

        for (auto elem : rule | std::views::reverse) {
            if (isTerminal(elem)) {
                trailer = firstSet.at(elem);
            } else {
                has_modifications =
                    followSetNonTerminalCase(rule, elem, trailer) || has_modifications;
            }
        }

        return has_modifications;
    }

    auto FollowSetEvaluator::followSetNonTerminalCase(
        Rule &rule, Symbol elem, std::unordered_set<Symbol> &trailer) -> bool
    {
        auto has_modifications = insertRange(followSet[elem], trailer);
        auto elem_first_set = firstSet.at(elem);

        if (elem_first_set.contains(epsilonSymbol)) {
            elem_first_set.erase(epsilonSymbol);
            insertRange(trailer, elem_first_set);
            rule.setFollowSet(trailer);
        } else {
            rule.setFollowSet(trailer);
            trailer = std::move(elem_first_set);
        }

        return has_modifications;
    }
}// namespace ccl::parser
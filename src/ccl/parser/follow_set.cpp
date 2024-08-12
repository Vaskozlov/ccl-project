#include "ccl/parser/follow_set.hpp"

namespace ccl::parser
{
    FollowSetEvaluator::FollowSetEvaluator(
        Symbol start_symbol, Symbol end_of_input, Symbol epsilon_symbol,
        const GrammarStorage &parser_rules,
        const std::unordered_map<Symbol, std::unordered_set<Symbol>> &first_set)
      : FirstAndFollowSetsCommon::FirstAndFollowSetsCommon{parser_rules}
      , firstSet{first_set}
      , epsilonSymbol{epsilon_symbol}
    {
        initializeFollowSet(start_symbol, end_of_input);
        computeFollowSet();
    }

    auto FollowSetEvaluator::initializeFollowSet(Symbol start_symbol, Symbol end_of_input) -> void
    {
        for (const auto &[key, rule] : grammarRules.rulesIterator()) {
            followSetOfRule.try_emplace(std::addressof(rule));
        }

        for (auto symbol : grammarRules.getGrammarSymbols()) {
            if (isNonTerminal(symbol)) {
                followSet.try_emplace(symbol);
            }
        }

        followSet.at(start_symbol).emplace(end_of_input);
    }

    auto FollowSetEvaluator::computeFollowSet() -> void
    {
        applyFixedPointAlgorithmOnAllRules([this](Symbol key, const Rule &rule) {
            return followSetComputationIteration(key, rule);
        });
    }

    auto FollowSetEvaluator::followSetComputationIteration(Symbol key, const Rule &rule) -> bool
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
        const Rule &rule, Symbol elem, std::unordered_set<Symbol> &trailer) -> bool
    {
        auto has_modifications = insertRange(followSet[elem], trailer);
        insertRange(followSetOfRule[std::addressof(rule)], trailer);
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
        const GrammarStorage &rules,
        const std::unordered_map<Symbol, std::unordered_set<Symbol>> &first_set)
        -> std::unordered_map<Symbol, std::unordered_set<Symbol>>
    {
        auto follow_set =
            FollowSetEvaluator(start_symbol, end_of_input, epsilon_symbol, rules, first_set);

        return std::move(follow_set.getFollowSet());
    }
}// namespace ccl::parser
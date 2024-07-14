#include "ccl/parser/follow_set.hpp"

namespace ccl::parser
{
    detail::FollowSetEvaluator::FollowSetEvaluator(
        Production start_symbol, Production end_of_input, Production epsilon_symbol,
        const isl::Set<Production> &grammar_symbols, const isl::Set<Production> &terminal_symbols,
        const isl::Map<Production, isl::Vector<isl::Vector<Production>>> &parser_rules,
        const isl::Map<Production, isl::Set<Production>> &first_set)
      : FirstAndFollowSetsCommon::
            FirstAndFollowSetsCommon{grammar_symbols, terminal_symbols, parser_rules}
      , firstSet{first_set}
      , epsilon{epsilon_symbol}
    {
        initializeFollowSet(start_symbol, end_of_input);
        computeFollowSet();
    }

    auto detail::FollowSetEvaluator::initializeFollowSet(
        Production start_symbol, Production end_of_input) -> void
    {
        for (auto symbol : symbols) {
            if (isNonTerminal(symbol)) {
                followSet.try_emplace(symbol);
            }
        }

        followSet.at(start_symbol).emplace(end_of_input);
    }

    auto detail::FollowSetEvaluator::computeFollowSet() -> void
    {
        applyFixedPointAlgorithmOnAllRules(
            [this](Production key, const isl::Vector<Production> &rule) {
                return followSetComputationIteration(key, rule);
            });
    }

    auto detail::FollowSetEvaluator::followSetComputationIteration(
        Production key, const isl::Vector<Production> &rule) -> bool
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
        Production elem, isl::Set<Production> &trailer) -> bool
    {
        auto has_modifications = insertRange(followSet[elem], trailer);
        auto elem_first_set = firstSet.at(elem);

        if (elem_first_set.contains(epsilon)) {
            elem_first_set.erase(epsilon);
            insertRange(trailer, elem_first_set);
        } else {
            trailer = std::move(elem_first_set);
        }

        return has_modifications;
    }

    auto evaluateFollowSet(
        Production start_symbol, Production end_of_input, Production epsilon_symbol,
        const isl::Set<Production> &grammar_symbols, const isl::Set<Production> &terminals,
        const isl::Map<Production, isl::Vector<isl::Vector<Production>>> &rules,
        const isl::Map<Production, isl::Set<Production>> &first_set)
        -> isl::Map<Production, isl::Set<Production>>
    {
        auto follow_set = detail::FollowSetEvaluator(
            start_symbol, end_of_input, epsilon_symbol, grammar_symbols, terminals, rules,
            first_set);

        return std::move(follow_set.getFollowSet());
    }
}// namespace ccl::parser
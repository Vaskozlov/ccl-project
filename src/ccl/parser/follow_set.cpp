#include "ccl/parser/follow_set.hpp"

namespace ccl::parser
{
    detail::FollowSetEvaluator::FollowSetEvaluator(
        Id start_symbol, Id end_of_input, Id epsilon_symbol, const isl::Set<Id> &grammar_symbols,
        const isl::Set<Id> &terminal_symbols,
        const isl::Map<Id, isl::Vector<isl::Vector<Id>>> &parser_rules,
        const isl::Map<Id, isl::Set<Id>> &first_set)
      : FirstAndFollowSetsCommon::
            FirstAndFollowSetsCommon{grammar_symbols, terminal_symbols, parser_rules}
      , firstSet{first_set}
      , epsilon{epsilon_symbol}
    {
        initializeFollowSet(start_symbol, end_of_input);
        computeFollowSet();
    }

    auto detail::FollowSetEvaluator::initializeFollowSet(Id start_symbol, Id end_of_input) -> void
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
        applyFixedPointAlgorithmOnAllRules([this](Id key, const isl::Vector<Id> &rule) {
            return followSetComputationIteration(key, rule);
        });
    }

    auto detail::FollowSetEvaluator::followSetComputationIteration(
        Id key, const isl::Vector<Id> &rule) -> bool
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

    auto
        detail::FollowSetEvaluator::followSetNonTerminalCase(Id elem, isl::Set<Id> &trailer) -> bool
    {
        auto has_modifications = insertRange(followSet[elem], trailer);
        auto elem_first_set = firstSet.at(elem);

        if (elem_first_set.contains(epsilon)) {
            elem_first_set.erase(epsilon);
            trailer.insert_range(elem_first_set);
        } else {
            trailer = std::move(elem_first_set);
        }

        return has_modifications;
    }

    auto evaluateFollowSet(
        Id start_symbol, Id end_of_input, Id epsilon_symbol, const isl::Set<Id> &grammar_symbols,
        const isl::Set<Id> &terminals, const isl::Map<Id, isl::Vector<isl::Vector<Id>>> &rules,
        const isl::Map<Id, isl::Set<Id>> &first_set) -> isl::Map<Id, isl::Set<Id>>
    {
        auto follow_set = detail::FollowSetEvaluator(
            start_symbol, end_of_input, epsilon_symbol, grammar_symbols, terminals, rules,
            first_set);

        return std::move(follow_set.getFollowSet());
    }
}// namespace ccl::parser
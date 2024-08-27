#ifndef CCL_PROJECT_FOLLOW_SET_HPP
#define CCL_PROJECT_FOLLOW_SET_HPP

#include <ccl/parser/detail/sets_common.hpp>

namespace ccl::parser
{
    class FollowSetEvaluator : private detail::FirstAndFollowSetsCommon
    {
    private:
        ankerl::unordered_dense::map<Symbol, std::unordered_set<Symbol>> followSet;
        const ankerl::unordered_dense::map<Symbol, std::unordered_set<Symbol>> &firstSet;
        Symbol epsilonSymbol;

        using FirstAndFollowSetsCommon::applyFixedPointAlgorithmOnAllRules;
        using FirstAndFollowSetsCommon::insertRange;
        using FirstAndFollowSetsCommon::isNonTerminal;
        using FirstAndFollowSetsCommon::isTerminal;

    public:
        FollowSetEvaluator(
            Symbol start_symbol, Symbol end_of_input, Symbol epsilon_symbol,
            GrammarStorage &parser_rules,
            const ankerl::unordered_dense::map<Symbol, std::unordered_set<Symbol>> &first_set);

        [[nodiscard]] auto getFollowSet() CCL_LIFETIMEBOUND -> auto &
        {
            return followSet;
        }

    private:
        auto initializeFollowSet(Symbol start_symbol, Symbol end_of_input) -> void;

        auto computeFollowSet() -> void;

        auto followSetComputationIteration(Symbol key, Rule &rule) -> bool;

        auto followSetNonTerminalCase(Rule &rule, Symbol elem, std::unordered_set<Symbol> &trailer)
            -> bool;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_FOLLOW_SET_HPP */

#ifndef CCL_PROJECT_FOLLOW_SET_HPP
#define CCL_PROJECT_FOLLOW_SET_HPP

#include <ccl/parser/detail/sets_common.hpp>

namespace ccl::parser
{
    class FollowSetEvaluator : private detail::FirstAndFollowSetsCommon
    {
    private:
        std::unordered_map<Symbol, std::unordered_set<Symbol>> followSet;
        std::unordered_map<const Rule *, std::unordered_set<Symbol>> followSetOfRule;
        const std::unordered_map<Symbol, std::unordered_set<Symbol>> &firstSet;
        Symbol epsilonSymbol;

        using FirstAndFollowSetsCommon::applyFixedPointAlgorithmOnAllRules;
        using FirstAndFollowSetsCommon::insertRange;
        using FirstAndFollowSetsCommon::isNonTerminal;
        using FirstAndFollowSetsCommon::isTerminal;

    public:
        FollowSetEvaluator(
            Symbol start_symbol, Symbol end_of_input, Symbol epsilon_symbol,
            const GrammarRulesStorage &parser_rules,
            const std::unordered_map<Symbol, std::unordered_set<Symbol>> &first_set);

        [[nodiscard]] auto getFollowSetOfRule() CCL_LIFETIMEBOUND -> auto &
        {
            return followSetOfRule;
        }

        [[nodiscard]] auto getFollowSet() CCL_LIFETIMEBOUND -> auto &
        {
            return followSet;
        }

    private:
        auto initializeFollowSet(Symbol start_symbol, Symbol end_of_input) -> void;

        auto computeFollowSet() -> void;

        auto followSetComputationIteration(Symbol key, const Rule &rule) -> bool;

        auto followSetNonTerminalCase(
            const Rule &rule, Symbol elem, std::unordered_set<Symbol> &trailer) -> bool;
    };

    auto evaluateFollowSet(
        Symbol start_symbol, Symbol end_of_input, Symbol epsilon_symbol,
        const GrammarRulesStorage &rules,
        const std::unordered_map<Symbol, std::unordered_set<Symbol>> &first_set)
        -> std::unordered_map<Symbol, std::unordered_set<Symbol>>;
}// namespace ccl::parser


#endif /* CCL_PROJECT_FOLLOW_SET_HPP */

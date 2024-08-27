#ifndef CCL_PROJECT_FIRST_SET_HPP
#define CCL_PROJECT_FIRST_SET_HPP

#include <ccl/parser/detail/sets_common.hpp>

namespace ccl::parser
{
    class FirstSetEvaluator : private detail::FirstAndFollowSetsCommon
    {
    private:
        ankerl::unordered_dense::map<Symbol, std::unordered_set<Symbol>> firstSet;
        Symbol epsilonSymbol;

        using FirstAndFollowSetsCommon::applyFixedPointAlgorithmOnAllRules;
        using FirstAndFollowSetsCommon::insertRange;
        using FirstAndFollowSetsCommon::isNonTerminal;
        using FirstAndFollowSetsCommon::isTerminal;

    public:
        FirstSetEvaluator(Symbol epsilon_symbol, GrammarStorage &parser_rules);

        [[nodiscard]] auto getFirstSet() CCL_LIFETIMEBOUND -> auto &
        {
            return firstSet;
        }

    private:
        auto initializeFirstSet() -> void;
        auto computeFirstSet() -> void;
        auto firstSetComputationIteration(Symbol key, Rule &rule) -> bool;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_FIRST_SET_HPP */

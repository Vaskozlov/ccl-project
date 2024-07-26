#ifndef CCL_PROJECT_FIRST_SET_HPP
#define CCL_PROJECT_FIRST_SET_HPP

#include <ccl/parser/detail/sets_common.hpp>

namespace ccl::parser
{
    namespace detail
    {
        class FirstSetEvaluator : private FirstAndFollowSetsCommon
        {
        private:
            std::map<Symbol, std::set<Symbol>> firstSet;
            Symbol epsilonSymbol;

            using FirstAndFollowSetsCommon::applyFixedPointAlgorithmOnAllRules;
            using FirstAndFollowSetsCommon::insertRange;
            using FirstAndFollowSetsCommon::isNonTerminal;
            using FirstAndFollowSetsCommon::isTerminal;

        public:
            FirstSetEvaluator(Symbol epsilon_symbol, const GrammarRulesStorage &parser_rules);

            [[nodiscard]] auto
                getFirstSet() CCL_LIFETIMEBOUND -> std::map<Symbol, std::set<Symbol>> &
            {
                return firstSet;
            }

        private:
            auto initializeFirstSet() -> void;
            auto computeFirstSet() -> void;
            auto firstSetComputationIteration(Symbol key, const Rule &rule) -> bool;
        };
    }// namespace detail

    auto evaluateFirstSet(Symbol epsilon, const GrammarRulesStorage &rules)
        -> std::map<Symbol, std::set<Symbol>>;
}// namespace ccl::parser

#endif /* CCL_PROJECT_FIRST_SET_HPP */

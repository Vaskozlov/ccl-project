#ifndef CCL_PROJECT_FOLLOW_SET_HPP
#define CCL_PROJECT_FOLLOW_SET_HPP

#include <ccl/parser/detail/sets_common.hpp>

namespace ccl::parser
{
    namespace detail
    {
        class FollowSetEvaluator : private FirstAndFollowSetsCommon
        {
        private:
            std::unordered_map<Symbol, std::unordered_set<Symbol>> followSet;
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

            [[nodiscard]] auto getFollowSet()
                CCL_LIFETIMEBOUND -> std::unordered_map<Symbol, std::unordered_set<Symbol>> &
            {
                return followSet;
            }

        private:
            auto initializeFollowSet(Symbol start_symbol, Symbol end_of_input) -> void;

            auto computeFollowSet() -> void;

            auto followSetComputationIteration(Symbol key, const Rule &rule) -> bool;

            auto followSetNonTerminalCase(Symbol elem, std::unordered_set<Symbol> &trailer) -> bool;
        };
    }// namespace detail

    auto evaluateFollowSet(
        Symbol start_symbol, Symbol end_of_input, Symbol epsilon_symbol,
        const GrammarRulesStorage &rules,
        const std::unordered_map<Symbol, std::unordered_set<Symbol>> &first_set)
        -> std::unordered_map<Symbol, std::unordered_set<Symbol>>;
}// namespace ccl::parser


#endif /* CCL_PROJECT_FOLLOW_SET_HPP */

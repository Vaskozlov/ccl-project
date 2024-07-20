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
            isl::Map<Symbol, isl::Set<Symbol>> firstSet;
            Symbol epsilonSymbol;

            using FirstAndFollowSetsCommon::applyFixedPointAlgorithmOnAllRules;
            using FirstAndFollowSetsCommon::insertRange;
            using FirstAndFollowSetsCommon::isNonTerminal;
            using FirstAndFollowSetsCommon::isTerminal;

        public:
            FirstSetEvaluator(
                Symbol epsilon_symbol, const isl::Set<Symbol> &grammar_symbols,
                const isl::Set<Symbol> &terminal_symbols, const GrammarRulesStorage &parser_rules);

            [[nodiscard]] auto
                getFirstSet() CCL_LIFETIMEBOUND -> isl::Map<Symbol, isl::Set<Symbol>> &
            {
                return firstSet;
            }

        private:
            auto initializeFirstSet() -> void;
            auto computeFirstSet() -> void;
            auto firstSetComputationIteration(Symbol key, const Rule &rule) -> bool;
        };
    }// namespace detail

    auto evaluateFirstSet(
        Symbol epsilon, const isl::Set<Symbol> &grammar_symbols, const isl::Set<Symbol> &terminals,
        const GrammarRulesStorage &rules) -> isl::Map<Symbol, isl::Set<Symbol>>;
}// namespace ccl::parser

#endif /* CCL_PROJECT_FIRST_SET_HPP */

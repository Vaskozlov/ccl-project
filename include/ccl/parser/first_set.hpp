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
            Symbol epsilon;

            using FirstAndFollowSetsCommon::applyFixedPointAlgorithmOnAllRules;
            using FirstAndFollowSetsCommon::insertRange;
            using FirstAndFollowSetsCommon::isNonTerminal;
            using FirstAndFollowSetsCommon::isTerminal;

        public:
            FirstSetEvaluator(
                Symbol epsilon_symbol, const isl::Set<Symbol> &grammar_symbols,
                const isl::Set<Symbol> &terminal_symbols,
                const isl::Map<Symbol, isl::Vector<isl::Vector<Symbol>>> &parser_rules);

            [[nodiscard]] auto
                getFirstSet() CCL_LIFETIMEBOUND -> isl::Map<Symbol, isl::Set<Symbol>> &
            {
                return firstSet;
            }

        private:
            auto initializeFirstSet() -> void;
            auto computeFirstSet() -> void;
            auto firstSetComputationIteration(Symbol key, const isl::Vector<Symbol> &rule) -> bool;
        };
    }// namespace detail

    auto evaluateFirstSet(
        Symbol epsilon, const isl::Set<Symbol> &grammar_symbols, const isl::Set<Symbol> &terminals,
        const isl::Map<Symbol, isl::Vector<isl::Vector<Symbol>>> &rules)
        -> isl::Map<Symbol, isl::Set<Symbol>>;
}// namespace ccl::parser

#endif /* CCL_PROJECT_FIRST_SET_HPP */

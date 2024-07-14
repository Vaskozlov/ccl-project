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
            isl::Map<Production, isl::Set<Production>> firstSet;
            Production epsilon;

            using FirstAndFollowSetsCommon::applyFixedPointAlgorithmOnAllRules;
            using FirstAndFollowSetsCommon::insertRange;
            using FirstAndFollowSetsCommon::isNonTerminal;
            using FirstAndFollowSetsCommon::isTerminal;
            using FirstAndFollowSetsCommon::symbols;
            using FirstAndFollowSetsCommon::terminals;

        public:
            FirstSetEvaluator(
                Production epsilon_symbol, const isl::Set<Production> &grammar_symbols,
                const isl::Set<Production> &terminal_symbols,
                const isl::Map<Production, isl::Vector<isl::Vector<Production>>> &parser_rules);

            [[nodiscard]] auto
                getFirstSet() CCL_LIFETIMEBOUND -> isl::Map<Production, isl::Set<Production>> &
            {
                return firstSet;
            }

        private:
            auto initializeFirstSet() -> void;
            auto computeFirstSet() -> void;
            auto firstSetComputationIteration(Production key, const isl::Vector<Production> &rule)
                -> bool;
        };
    }// namespace detail

    auto evaluateFirstSet(
        Production epsilon, const isl::Set<Production> &grammar_symbols,
        const isl::Set<Production> &terminals,
        const isl::Map<Production, isl::Vector<isl::Vector<Production>>> &rules)
        -> isl::Map<Production, isl::Set<Production>>;
}// namespace ccl::parser

#endif /* CCL_PROJECT_FIRST_SET_HPP */

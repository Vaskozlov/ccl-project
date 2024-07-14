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
            isl::Map<Production, isl::Set<Production>> followSet;
            const isl::Map<Production, isl::Set<Production>> &firstSet;
            Production epsilon;

            using FirstAndFollowSetsCommon::applyFixedPointAlgorithmOnAllRules;
            using FirstAndFollowSetsCommon::insertRange;
            using FirstAndFollowSetsCommon::isNonTerminal;
            using FirstAndFollowSetsCommon::isTerminal;
            using FirstAndFollowSetsCommon::symbols;
            using FirstAndFollowSetsCommon::terminals;

        public:
            FollowSetEvaluator(
                Production start_symbol, Production end_of_input, Production epsilon_symbol,
                const isl::Set<Production> &grammar_symbols, const isl::Set<Production> &terminal_symbols,
                const isl::Map<Production, isl::Vector<isl::Vector<Production>>> &parser_rules,
                const isl::Map<Production, isl::Set<Production>> &first_set);

            [[nodiscard]] auto getFollowSet() CCL_LIFETIMEBOUND -> isl::Map<Production, isl::Set<Production>> &
            {
                return followSet;
            }

        private:
            auto initializeFollowSet(Production start_symbol, Production end_of_input) -> void;

            auto computeFollowSet() -> void;

            auto followSetComputationIteration(Production key, const isl::Vector<Production> &rule) -> bool;

            auto followSetNonTerminalCase(Production elem, isl::Set<Production> &trailer) -> bool;
        };
    }// namespace detail

    auto evaluateFollowSet(
        Production start_symbol, Production end_of_input, Production epsilon_symbol, const isl::Set<Production> &grammar_symbols,
        const isl::Set<Production> &terminals, const isl::Map<Production, isl::Vector<isl::Vector<Production>>> &rules,
        const isl::Map<Production, isl::Set<Production>> &first_set) -> isl::Map<Production, isl::Set<Production>>;
}// namespace ccl::parser


#endif /* CCL_PROJECT_FOLLOW_SET_HPP */

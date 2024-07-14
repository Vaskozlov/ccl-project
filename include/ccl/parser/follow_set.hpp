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
            isl::Map<Id, isl::Set<Id>> followSet;
            const isl::Map<Id, isl::Set<Id>> &firstSet;
            Id epsilon;

            using FirstAndFollowSetsCommon::applyFixedPointAlgorithmOnAllRules;
            using FirstAndFollowSetsCommon::insertRange;
            using FirstAndFollowSetsCommon::isNonTerminal;
            using FirstAndFollowSetsCommon::isTerminal;
            using FirstAndFollowSetsCommon::symbols;
            using FirstAndFollowSetsCommon::terminals;

        public:
            FollowSetEvaluator(
                Id start_symbol, Id end_of_input, Id epsilon_symbol,
                const isl::Set<Id> &grammar_symbols, const isl::Set<Id> &terminal_symbols,
                const isl::Map<Id, isl::Vector<isl::Vector<Id>>> &parser_rules,
                const isl::Map<Id, isl::Set<Id>> &first_set);

            [[nodiscard]] auto getFollowSet() -> isl::Map<Id, isl::Set<Id>> &
            {
                return followSet;
            }

        private:
            auto initializeFollowSet(Id start_symbol, Id end_of_input) -> void;

            auto computeFollowSet() -> void;

            auto followSetComputationIteration(Id key, const isl::Vector<Id> &rule) -> bool;

            auto followSetNonTerminalCase(Id elem, isl::Set<Id> &trailer) -> bool;
        };
    }// namespace detail

    auto evaluateFollowSet(
        Id start_symbol, Id end_of_input, Id epsilon_symbol, const isl::Set<Id> &grammar_symbols,
        const isl::Set<Id> &terminals, const isl::Map<Id, isl::Vector<isl::Vector<Id>>> &rules,
        const isl::Map<Id, isl::Set<Id>> &first_set) -> isl::Map<Id, isl::Set<Id>>;
}// namespace ccl::parser


#endif /* CCL_PROJECT_FOLLOW_SET_HPP */

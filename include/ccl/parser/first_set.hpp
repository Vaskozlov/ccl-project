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
            isl::Map<Id, isl::Set<Id>> firstSet;
            std::size_t epsilon;

            using FirstAndFollowSetsCommon::applyFixedPointAlgorithmOnAllRules;
            using FirstAndFollowSetsCommon::insertRange;
            using FirstAndFollowSetsCommon::isNonTerminal;
            using FirstAndFollowSetsCommon::isTerminal;
            using FirstAndFollowSetsCommon::symbols;
            using FirstAndFollowSetsCommon::terminals;

        public:
            FirstSetEvaluator(
                Id epsilon_symbol, const isl::Set<Id> &grammar_symbols,
                const isl::Set<Id> &terminal_symbols,
                const isl::Map<Id, isl::Vector<isl::Vector<Id>>> &parser_rules);

            [[nodiscard]] auto getFirstSet() -> isl::Map<Id, isl::Set<Id>> &
            {
                return firstSet;
            }

        private:
            auto initializeFirstSet() -> void;
            auto computeFirstSet() -> void;
            auto firstSetComputationIteration(Id key, const isl::Vector<Id> &rule) -> bool;
        };
    }// namespace detail

    auto evaluateFirstSet(
        Id epsilon, const isl::Set<Id> &grammar_symbols, const isl::Set<Id> &terminals,
        const isl::Map<Id, isl::Vector<isl::Vector<Id>>> &rules) -> isl::Map<Id, isl::Set<Id>>;
}// namespace ccl::parser

#endif /* CCL_PROJECT_FIRST_SET_HPP */

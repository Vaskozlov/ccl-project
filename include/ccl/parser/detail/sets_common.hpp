#ifndef CCL_PROJECT_SETS_COMMON_HPP
#define CCL_PROJECT_SETS_COMMON_HPP

#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/types.hpp>

namespace ccl::parser::detail
{
    class FirstAndFollowSetsCommon
    {
    protected:
        const GrammarStorage &grammarRules;// NOLINT reference

        static auto insertRange(auto &&set, isl::RangeOf<Symbol> auto &&range) -> bool
        {
            auto has_inserted_element = false;

            for (auto &&elem : range) {
                auto has_inserted = false;

                std::tie(std::ignore, has_inserted) =
                    set.insert(std::forward<decltype(elem)>(elem));

                has_inserted_element = has_inserted || has_inserted_element;
            }

            return has_inserted_element;
        }

    public:
        explicit FirstAndFollowSetsCommon(const GrammarStorage &grammar_rules)
          : grammarRules{grammar_rules}
        {}

        [[nodiscard]] auto isTerminal(Symbol symbol) const -> bool
        {
            return grammarRules.isTerminal(symbol);
        }

        [[nodiscard]] auto isNonTerminal(Symbol symbol) const -> bool
        {
            return !isTerminal(symbol);
        }

        template<typename F>
        auto applyFixedPointAlgorithmOnAllRules(F &&function) -> void
        {
            auto has_modifications = true;

            while (has_modifications) {
                has_modifications = false;

                for (const auto &[key, rule] : grammarRules.rulesIterator()) {
                    has_modifications = function(key, rule) || has_modifications;
                }
            }
        }
    };
}// namespace ccl::parser::detail

#endif /* CCL_PROJECT_SETS_COMMON_HPP */

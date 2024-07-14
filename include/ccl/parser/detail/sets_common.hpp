#ifndef CCL_PROJECT_SETS_COMMON_HPP
#define CCL_PROJECT_SETS_COMMON_HPP

#include <ccl/parser/general.hpp>
#include <map>
#include <set>

namespace ccl::parser::detail
{
    class FirstAndFollowSetsCommon
    {
    protected:
        const isl::Map<Symbol, isl::Vector<isl::Vector<Symbol>>> &grammarRules;// NOLINT reference
        const isl::Set<Symbol> &allSymbolsInGrammar;                           // NOLINT reference
        const isl::Set<Symbol> &terminalSymbols;                               // NOLINT reference

        static auto insertRange(isl::Set<Symbol> &set, isl::RangeOf<Symbol> auto &&range) -> bool
        {
            auto has_inserted_element = false;

            for (auto &&elem : range) {
                auto [it, has_inserted] = set.insert(std::forward<decltype(elem)>(elem));
                has_inserted_element = has_inserted || has_inserted_element;
            }

            return has_inserted_element;
        }

    public:
        FirstAndFollowSetsCommon(
            const isl::Set<Symbol> &grammar_symbols,
            const isl::Set<Symbol> &terminal_symbols,
            const isl::Map<Symbol, isl::Vector<isl::Vector<Symbol>>> &grammar_rules)
          : grammarRules{grammar_rules}
          , allSymbolsInGrammar{grammar_symbols}
          , terminalSymbols{terminal_symbols}
        {}

        [[nodiscard]] auto isTerminal(Symbol value) const -> bool
        {
            return terminalSymbols.contains(value);
        }

        [[nodiscard]] auto isNonTerminal(Symbol value) const -> bool
        {
            return !isTerminal(value);
        }

        template<typename F>
        auto applyFixedPointAlgorithmOnAllRules(F &&function) -> void
        {
            auto has_modifications = true;

            while (has_modifications) {
                has_modifications = false;

                for (const auto &[key, rule_alternatives] : grammarRules) {
                    for (const auto &rule : rule_alternatives) {
                        has_modifications = function(key, rule) || has_modifications;
                    }
                }
            }
        }
    };
}// namespace ccl::parser::detail

#endif /* CCL_PROJECT_SETS_COMMON_HPP */

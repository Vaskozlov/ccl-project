#ifndef CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP
#define CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP

#include "rule.hpp"
#include <ccl/parser/types.hpp>
#include <isl/generator.hpp>

namespace ccl::parser
{
    class GrammarRulesStorage : public std::map<Symbol, std::vector<Rule>>
    {
    private:
        using AlternativesConstRuleIterator = typename std::vector<Rule>::const_iterator;

        std::set<Symbol> nonTerminals;
        std::set<Symbol> grammarSymbols;
        std::set<Symbol> possiblyEmptyRules;
        Symbol epsilonSymbol;

    public:
        explicit GrammarRulesStorage(Symbol epsilon);

        GrammarRulesStorage(
            Symbol epsilon,
            const std::initializer_list<isl::Pair<Symbol, std::vector<Rule>>> &initial_data);

        auto finishGrammar() -> void;

        [[nodiscard]] auto getEpsilon() const noexcept -> Symbol
        {
            return epsilonSymbol;
        }

        [[nodiscard]] auto getNonTerminals() const noexcept -> const std::set<Symbol> &
        {
            return nonTerminals;
        }

        [[nodiscard]] auto getGrammarSymbols() const noexcept -> const std::set<Symbol> &
        {
            return grammarSymbols;
        }

        [[nodiscard]] auto isTerminal(Symbol symbol) const noexcept -> bool
        {
            return !nonTerminals.contains(symbol);
        }

        [[nodiscard]] auto rulesIterator() -> isl::Generator<isl::Pair<Symbol, Rule &>>
        {
            for (auto &[key, rule_alternatives] : *this) {
                for (auto &rule : rule_alternatives) {
                    co_yield isl::Pair<Symbol, Rule &>{key, rule};
                }
            }
        }

        [[nodiscard]] auto rulesIterator() const -> isl::Generator<isl::Pair<Symbol, const Rule &>>
        {
            for (const auto &[key, rule_alternatives] : *this) {
                for (const auto &rule : rule_alternatives) {
                    co_yield isl::Pair<Symbol, const Rule &>{key, rule};
                }
            }
        }

        [[nodiscard]] auto rulesIteratorWithIterators() const
            -> isl::Generator<isl::Pair<Symbol, AlternativesConstRuleIterator>>
        {
            for (const auto &[key, rule_alternatives] : *this) {
                for (auto rule_it = rule_alternatives.cbegin(); rule_it != rule_alternatives.cend();
                     ++rule_it) {
                    co_yield isl::Pair<Symbol, AlternativesConstRuleIterator>{key, rule_it};
                }
            }
        }

        [[nodiscard]] auto getNotFilledHandlers(Symbol start_symbol, Symbol end_symbol) const noexcept -> std::set<Symbol>;

    private:
        auto registerAllRuleSymbols(const Rule &rule) -> void;

        auto findAndFixEmptyRules() -> void;

        auto findEmptyRules() -> void;

        auto fixEmptyRules() -> void;

        auto fixEmptyRulesIteration(
            Symbol &production, std::optional<Rule> &rule_to_add,
            std::optional<Rule> &rule_to_remove,
            std::unordered_map<Symbol, std::unordered_map<Rule, std::size_t>> &fixed_rules_part)
            -> bool;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP */

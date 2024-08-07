#ifndef CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP
#define CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP

#include <ccl/parser/rule.hpp>
#include <ccl/parser/types.hpp>
#include <isl/coroutine/generator.hpp>
#include <list>

namespace ccl::parser
{
    class GrammarRulesStorage : private std::unordered_map<Symbol, std::vector<Rule>>
    {
    private:
        using Self = std::unordered_map<Symbol, std::vector<Rule>>;
        using AlternativesConstRuleIterator = typename std::vector<Rule>::const_iterator;

        std::unordered_set<Symbol> nonTerminals;
        std::unordered_set<Symbol> grammarSymbols;
        std::unordered_set<Symbol> possiblyEmptyRules;
        Symbol epsilonSymbol;

    public:
        explicit GrammarRulesStorage(Symbol epsilon);

        GrammarRulesStorage(
            Symbol epsilon,
            const std::initializer_list<isl::Pair<Symbol, std::vector<Rule>>> &initial_data);

        auto finishGrammar() -> void;

        auto tryEmplace(Symbol key, Rule rule) -> bool;

        auto eraseRule(Symbol key, const Rule &rule) -> void;

        [[nodiscard]] auto at(Symbol key) const -> const std::vector<Rule> &
        {
            return Self::at(key);
        }

        [[nodiscard]] auto getEpsilon() const noexcept -> Symbol
        {
            return epsilonSymbol;
        }

        [[nodiscard]] auto getNonTerminals() const noexcept -> const std::unordered_set<Symbol> &
        {
            return nonTerminals;
        }

        [[nodiscard]] auto getGrammarSymbols() const noexcept -> const std::unordered_set<Symbol> &
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

    private:
        auto registerAllRuleSymbols(const Rule &rule) -> void;

        auto findAndFixEmptyRules() -> void;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP */

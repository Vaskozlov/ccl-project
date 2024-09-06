#ifndef CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP
#define CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP

#include <ccl/parser/rule.hpp>
#include <ccl/parser/types.hpp>
#include <isl/coroutine/generator.hpp>
#include <unordered_map>
#include <unordered_set>

namespace ccl::parser
{
    class GrammarStorage
    {
    private:
        using AlternativesConstRuleIterator = std::vector<Rule>::const_iterator;

        std::unordered_map<Symbol, std::vector<Rule>> rules;
        ankerl::unordered_dense::set<Symbol> nonTerminals;
        ankerl::unordered_dense::set<Symbol> grammarSymbols;
        ankerl::unordered_dense::set<Symbol> possiblyEmptyRules;
        ankerl::unordered_dense::map<Symbol, std::unordered_set<Symbol>> firstSet;
        ankerl::unordered_dense::map<Symbol, std::unordered_set<Symbol>> followSet;
        Symbol epsilonSymbol;

    public:
        explicit GrammarStorage(Symbol epsilon);

        GrammarStorage(
            bool remove_epsilon,
            Symbol epsilon,
            Symbol goal_production,
            const std::initializer_list<isl::Pair<Symbol, std::vector<Rule>>> &initial_data);

        auto finishGrammar(Symbol goal_production, bool remove_epsilon) -> void;

        auto tryEmplace(Symbol key, Rule rule) -> bool;

        auto eraseRule(Symbol key, const Rule &rule) -> void;

        [[nodiscard]] auto at(const Symbol key) const CCL_LIFETIMEBOUND -> const auto &
        {
            return rules.at(key);
        }

        [[nodiscard]] auto getEpsilon() const noexcept -> Symbol
        {
            return epsilonSymbol;
        }

        [[nodiscard]] auto getNonTerminals() const noexcept CCL_LIFETIMEBOUND -> const auto &
        {
            return nonTerminals;
        }

        [[nodiscard]] auto getGrammarSymbols() const noexcept CCL_LIFETIMEBOUND -> const auto &
        {
            return grammarSymbols;
        }

        [[nodiscard]] auto getFirstSet() const noexcept CCL_LIFETIMEBOUND -> const auto &
        {
            return firstSet;
        }

        [[nodiscard]] auto getFollowSet() const noexcept CCL_LIFETIMEBOUND -> const auto &
        {
            return followSet;
        }

        [[nodiscard]] auto
            getStartRule(Symbol start_production_id) const CCL_LIFETIMEBOUND -> const Rule *;

        [[nodiscard]] auto isTerminal(const Symbol symbol) const noexcept -> bool
        {
            return !nonTerminals.contains(symbol);
        }

        [[nodiscard]] auto
            rulesIterator() CCL_LIFETIMEBOUND -> isl::Generator<isl::Pair<Symbol, Rule &>>
        {
            for (auto &[key, rule_alternatives] : rules) {
                for (auto &rule : rule_alternatives) {
                    co_yield isl::Pair<Symbol, Rule &>{key, rule};
                }
            }
        }

        [[nodiscard]] auto rulesIterator() const CCL_LIFETIMEBOUND
            -> isl::Generator<isl::Pair<Symbol, const Rule &>>
        {
            for (const auto &[key, rule_alternatives] : rules) {
                for (const auto &rule : rule_alternatives) {
                    co_yield isl::Pair<Symbol, const Rule &>{key, rule};
                }
            }
        }

    private:
        auto registerAllRuleSymbols(const Rule &rule) -> void;

        auto findAndFixEmptyRules() -> void;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP */

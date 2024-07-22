#ifndef CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP
#define CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP

#include <ccl/parser/lr/rule.hpp>
#include <ccl/parser/types.hpp>
#include <isl/generator.hpp>

namespace ccl::parser
{
    class GrammarRulesStorage : public isl::Map<Symbol, isl::Vector<Rule>>
    {
    private:
        isl::Set<Symbol> nonTerminals;
        isl::Set<Symbol> grammarSymbols{0};

    public:
        GrammarRulesStorage() = default;

        GrammarRulesStorage(
            const std::initializer_list<std::pair<Symbol, isl::Vector<Rule>>> &initial_data)
          : isl::Map<Symbol, isl::Vector<Rule>>{initial_data}
        {
            finishGrammar();
        }

        auto finishGrammar() -> void
        {
            for (const auto &[key, rules] : *this) {
                nonTerminals.emplace(key);
                grammarSymbols.emplace(key);

                for (const auto &rule : rules) {
                    registerAllRuleSymbols(rule);
                }
            }
        }

        auto getTerminalsRange() -> auto
        {
            return std::ranges::filter_view(grammarSymbols, [this](Symbol symbol) {
                return isTerminal(symbol);
            });
        }

        [[nodiscard]] auto getNonTerminals() const noexcept -> const isl::Set<Symbol> &
        {
            return nonTerminals;
        }

        [[nodiscard]] auto getGrammarSymbols() const noexcept -> const isl::Set<Symbol> &
        {
            return grammarSymbols;
        }

        [[nodiscard]] auto isTerminal(Symbol symbol) const noexcept -> bool
        {
            return !nonTerminals.contains(symbol);
        }

        [[nodiscard]] auto rulesIterator() const -> isl::Generator<isl::Pair<Symbol, const Rule &>>
        {
            for (const auto &[key, rule_alternatives] : *this) {
                for (const auto &rule : rule_alternatives) {
                    co_yield isl::Pair<Symbol, const Rule &>{key, rule};
                }
            }
        }

    private:
        auto registerAllRuleSymbols(const Rule &rule) -> void
        {
            for (const auto symbol : rule) {
                grammarSymbols.emplace(symbol);
            }
        }
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP */

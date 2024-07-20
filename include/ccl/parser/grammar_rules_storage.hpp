#ifndef CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP
#define CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP

#include <ccl/parser/lr/rule.hpp>
#include <ccl/parser/types.hpp>
#include <isl/generator.hpp>

namespace ccl::parser
{
    class GrammarRulesStorage : public isl::Map<Symbol, isl::Vector<Rule>>
    {
    public:
        using isl::Map<Symbol, isl::Vector<Rule>>::Map;

        [[nodiscard]] auto rulesIterator() const -> isl::Generator<isl::Pair<Symbol, const Rule &>>
        {
            for (const auto &[key, rule_alternatives] : *this) {
                for (const auto &rule : rule_alternatives) {
                    co_yield isl::Pair<Symbol, const Rule &>{key, rule};
                }
            }
        }
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_GRAMMAR_RULES_STORAGE_HPP */

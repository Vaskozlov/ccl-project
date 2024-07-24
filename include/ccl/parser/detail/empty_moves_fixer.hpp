#ifndef CCL_PROJECT_EMPTY_MOVES_FIXER_HPP
#define CCL_PROJECT_EMPTY_MOVES_FIXER_HPP

#include <ccl/parser/grammar_rules_storage.hpp>

namespace ccl::parser::detail
{
    class EmptyRulesFixer
    {
    private:
        isl::UnorderedMap<Symbol, isl::UnorderedMap<Rule, std::size_t>> scannedParts;
        isl::Set<Symbol> possiblyEmptyRules;
        isl::Vector<isl::Pair<Symbol, Rule>> pendingRulesToAdd;
        isl::Vector<isl::Pair<Symbol, Rule>> pendingRulesToRemove;
        GrammarRulesStorage &storage;

    public:
        EmptyRulesFixer(GrammarRulesStorage &grammar_storage);

        auto apply() -> void;

    private:
        auto findEmptyRules() -> void;

        auto fixEmptyRules() -> void;

        auto fixRule(Symbol key, const Rule &rule, std::size_t current_symbol_index) -> void;
    };

}// namespace ccl::parser::detail

#endif /* CCL_PROJECT_EMPTY_MOVES_FIXER_HPP */

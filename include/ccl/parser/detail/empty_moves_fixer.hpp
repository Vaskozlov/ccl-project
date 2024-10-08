#ifndef CCL_PROJECT_EMPTY_MOVES_FIXER_HPP
#define CCL_PROJECT_EMPTY_MOVES_FIXER_HPP

#include <ccl/parser/grammar_rules_storage.hpp>

namespace ccl::parser::detail
{
    class EmptyMovesFixer
    {
    private:
        ankerl::unordered_dense::set<Symbol> possiblyEmptyRules;
        std::vector<isl::Pair<Symbol, Rule>> pendingRulesToAdd;
        std::vector<isl::Pair<Symbol, Rule>> pendingRulesToRemove;
        GrammarStorage &storage;

    public:
        explicit EmptyMovesFixer(GrammarStorage &grammar_storage);

        auto apply() -> void;

    private:
        auto findEmptyRules() -> void;

        auto fixEmptyRules() -> void;

        auto fixRule(Symbol key, const Rule &rule, std::size_t current_symbol_index) -> void;
    };

}// namespace ccl::parser::detail

#endif /* CCL_PROJECT_EMPTY_MOVES_FIXER_HPP */

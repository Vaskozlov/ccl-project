#ifndef CCL_PROJECT_LR_PARSER_HPP
#define CCL_PROJECT_LR_PARSER_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/lr/action.hpp>
#include <ccl/parser/lr/canonical_collection.hpp>
#include <ccl/parser/lr/table_entry.hpp>

namespace ccl::parser
{
    class LrParser
    {
    private:
        isl::Map<TableEntry, State> gotoTable;
        isl::Map<TableEntry, Action> actionTable;

    public:
        explicit LrParser(
            const LrItem &start_item, Symbol epsilon_symbol,
            const isl::Set<Symbol> &grammar_symbols, const isl::Set<Symbol> &terminal_symbols,
            const GrammarRulesStorage &parser_rules);

        auto parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const -> ast::NodePtr;

        [[nodiscard]] auto getGotoTable() const noexcept -> const isl::Map<TableEntry, State> &
        {
            return gotoTable;
        }

        [[nodiscard]] auto getActionTable() const noexcept -> const isl::Map<TableEntry, Action> &
        {
            return actionTable;
        }

    private:
        auto reduceAction(
            const Action &action,
            isl::Vector<State> &state_stack,
            isl::Vector<ast::NodePtr> &nodes_stack) const -> void;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_LR_PARSER_HPP */

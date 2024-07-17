#ifndef CCL_PROJECT_GLR_PARSER_HPP
#define CCL_PROJECT_GLR_PARSER_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/lr/action.hpp>
#include <ccl/parser/lr/detail//canonical_collection.hpp>
#include <ccl/parser/lr/detail//table_entry.hpp>
#include <isl/weak_stack.hpp>
#include <list>

namespace ccl::parser
{
    class GlrParser
    {
    private:
        isl::Map<TableEntry, State> gotoTable;
        isl::Map<TableEntry, isl::Vector<Action>> actionTable;

    public:
        explicit GlrParser(
            const LrItem &start_item, Symbol epsilon_symbol,
            const isl::Set<Symbol> &grammar_symbols, const isl::Set<Symbol> &terminal_symbols,
            const GrammarRulesStorage &parser_rules);

        [[nodiscard]] auto parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const
            -> isl::Vector<ast::ShNodePtr>;

        [[nodiscard]] auto getGotoTable() const noexcept -> const isl::Map<TableEntry, State> &
        {
            return gotoTable;
        }

        [[nodiscard]] auto
            getActionTable() const noexcept -> const isl::Map<TableEntry, isl::Vector<Action>> &
        {
            return actionTable;
        }
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_GLR_PARSER_HPP */

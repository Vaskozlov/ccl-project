#ifndef CCL_PROJECT_GLR_PARSER_HPP
#define CCL_PROJECT_GLR_PARSER_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/lr/action.hpp>
#include <ccl/parser/lr/detail/canonical_collection.hpp>
#include <ccl/parser/table_entry.hpp>
#include <isl/gss.hpp>

namespace ccl::parser
{
    class GlrParser
    {
    private:
        ankerl::unordered_dense::map<TableEntry, State> gotoTable;
        ankerl::unordered_dense::map<TableEntry, std::vector<Action>> actionTable;
        std::function<std::string(SmallId)> idToStringConverter;

    public:
        explicit GlrParser(
            const GrammarSlot &start_item,
            Symbol epsilon_symbol,
            const GrammarStorage &parser_rules,
            std::function<std::string(SmallId)> id_to_string_converter = fmt::to_string<SmallId>);

        [[nodiscard]] auto parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const
            -> std::pair<GSStack, isl::DynamicForwardList<ast::Node>>;

        [[nodiscard]] auto getGotoTable() const noexcept -> const auto &
        {
            return gotoTable;
        }

        [[nodiscard]] auto getActionTable() const noexcept -> const auto &
        {
            return actionTable;
        }
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_GLR_PARSER_HPP */

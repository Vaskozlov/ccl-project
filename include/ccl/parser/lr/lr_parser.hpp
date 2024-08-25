#ifndef CCL_PROJECT_LR_PARSER_HPP
#define CCL_PROJECT_LR_PARSER_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/ast/node.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/lr/action.hpp>
#include <ccl/parser/parsing_result.hpp>
#include <ccl/parser/table_entry.hpp>

#include <ccl/parser/lr/detail/lr_parser_generator.hpp>

namespace ccl::parser
{
    class LrParser
    {
    private:
        struct ParserState
        {
            Stack<State> stateStack{};
            Stack<ast::SharedNode<>> nodesStack{};
        };

        lr::LrGotoTable gotoTable;
        lr::Lr1ActionTable actionTable;

    public:
        explicit LrParser(
            const LrItem &start_item,
            Symbol epsilon_symbol,
            const GrammarStorage &parser_rules,
            const std::function<std::string(SmallId)> &id_to_string_converter =
                fmt::to_string<SmallId>);

        [[nodiscard]] auto
            parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const -> UnambiguousParsingResult;

        [[nodiscard]] auto getGotoTable() const noexcept -> const auto &
        {
            return gotoTable;
        }

        [[nodiscard]] auto getActionTable() const noexcept -> const auto &
        {
            return actionTable;
        }

    private:
        static auto
            shiftAction(const lexer::Token *word, State shifting_state, ParserState &parser_state)
                -> void;

        auto reduceAction(const LrItem &lr_item, ParserState &parser_state) const -> void;
    };
}// namespace ccl::parser

#endif /* CCL_PROJECT_LR_PARSER_HPP */

#ifndef CCL_PROJECT_GLR_PARSER_HPP
#define CCL_PROJECT_GLR_PARSER_HPP

#include <ccl/lexer/lexical_analyzer.hpp>
#include <ccl/parser/grammar_rules_storage.hpp>
#include <ccl/parser/lr/action.hpp>
#include <ccl/parser/lr/detail/lr_parser_generator.hpp>
#include <ccl/parser/parsing_result.hpp>

namespace ccl::parser
{
    class GlrParser
    {
    private:
        lr::LrGotoTable gotoTable;
        lr::GlrActionTable actionTable;
        std::function<std::string(SmallId)> idToStringConverter;

    public:
        explicit GlrParser(
            const LrItem &start_item,
            Symbol epsilon_symbol,
            const GrammarStorage &parser_rules,
            const std::function<std::string(SmallId)> &id_to_string_converter =
                fmt::to_string<SmallId>);

        [[nodiscard]] auto
            parse(lexer::LexicalAnalyzer::Tokenizer &tokenizer) const -> AmbiguousParsingResult;

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

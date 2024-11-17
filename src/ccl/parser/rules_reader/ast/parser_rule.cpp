#include <ccl/lexer/rule/sequence.hpp>
#include <ccl/parser/ast/terminal.hpp>
#include <ccl/parser/rules_reader/ast/parser_rule.hpp>
#include <ccl/parser/rules_reader/rules_lexer.hpp>
#include <ccl/text/remove_escaping.hpp>

namespace ccl::parser::reader::ast
{
    static auto addStringToLexicalAnalyzer(
        const std::string &str,
        ParserBuilder &parser_builder,
        bool is_any_place_string) -> void
    {
        if (parser_builder.getLexicalAnalyzer().hasRule(str)) {
            return;
        }

        const auto rule_id = parser_builder.addRule(str);

        auto container = std::make_unique<lexer::rule::Container>(
            parser_builder.getLexicalAnalyzer(), false, is_any_place_string);

        container->setId(rule_id);

        const auto str_without_quotes = str.substr(1, str.size() - 2);

        container->addItem(
            std::make_unique<lexer::rule::Sequence>(text::removeEscaping(str_without_quotes, {})));

        parser_builder.addLexerRule(str, std::move(container));
    }

    auto ParserRule::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        auto result = std::vector<SmallId>{};
        const auto *parser_rule_block = static_cast<const parser::ast::Terminal *>(front().get());
        const auto token_type = parser_rule_block->getType();
        const auto repr = parser_rule_block->getToken().getRepr();

        if (token_type == STRING || token_type == ANY_PLACE_STRING) {
            addStringToLexicalAnalyzer(
                static_cast<std::string>(repr), parser_builder, token_type == ANY_PLACE_STRING);
        }

        result.emplace_back(parser_builder.addRule(static_cast<std::string>(repr)));

        if (size() == 2) {
            const auto *next_node = static_cast<const RulesReaderNode *>(back().get());

            const auto next_result =
                isl::get<std::vector<SmallId>>(next_node->construct(parser_builder));

            result.insert(result.end(), next_result.begin(), next_result.end());
        }

        return isl::UniqueAny{std::move(result)};
    }
}// namespace ccl::parser::reader::ast
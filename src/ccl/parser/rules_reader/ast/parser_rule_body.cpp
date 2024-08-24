#include <ccl/lexer/rule/sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/rules_reader/ast/parser_rule_body.hpp>
#include <ccl/parser/rules_reader/rules_lexer.hpp>
#include <ccl/text/remove_escaping.hpp>

namespace ccl::parser::reader::ast
{
    auto ParserRuleBody::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        auto result = std::vector<SmallId>{};

        for (const auto &node : nodes) {
            const auto *node_as_token = dynamic_cast<const parser::ast::TokenNode *>(node.get());
            const auto &token = node_as_token->getToken();
            const auto repr = token.getRepr();

            if (token.getId() == STRING || token.getId() == ANY_PLACE_STRING) {
                addStringToLexicalAnalyzer(repr, parser_builder, token.getId() == ANY_PLACE_STRING);
            }

            result.emplace_back(parser_builder.addRule(repr));
        }

        return isl::UniqueAny{std::move(result)};
    }

    auto ParserRuleBody::addStringToLexicalAnalyzer(
        isl::string_view str,
        ParserBuilder &parser_builder,
        bool is_any_place_string) const -> void
    {
        if (parser_builder.hasRule(str)) {
            return;
        }

        auto rule_id = parser_builder.addRule(str);

        auto container = std::make_unique<lexer::rule::Container>(
            parser_builder.getLexicalAnalyzer(), false, is_any_place_string);

        container->setId(rule_id);

        auto str_without_quotes = str.substr(1, str.size() - 2);
        container->addItem(
            std::make_unique<lexer::rule::Sequence>(text::removeEscaping(str_without_quotes, {})));

        parser_builder.addLexerRule(str, std::move(container));
    }
}// namespace ccl::parser::reader::ast
#include <ccl/lexer/rule/sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/rules_reader/ast/parser_rule_body.hpp>
#include <ccl/text/remove_escaping.hpp>

namespace ccl::parser::reader::ast
{
    auto ParserRuleBody::construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny
    {
        auto result = isl::Vector<Id>{};

        for (const auto &node : nodes) {
            const auto *node_as_token = dynamic_cast<const parser::ast::TokenNode *>(node.get());
            const auto &token = node_as_token->getToken();
            const auto repr = token.getRepr();

            if (token.getId() == RulesLexerToken::STRING) {
                addStringToLexicalAnalyzer(repr, rule_constructor);
            }

            result.emplace_back(rule_constructor.addRule(repr));
        }

        return isl::UniqueAny{std::move(result)};
    }

    auto ParserRuleBody::addStringToLexicalAnalyzer(
        isl::string_view str, RulesConstructor &rule_constructor) const -> void
    {
        if (rule_constructor.hasRule(str)) {
            return;
        }

        auto rule_id = rule_constructor.addRule(str);
        auto container = isl::makeUnique<lexer::rule::Container>(
            rule_constructor.getLexicalAnalyzer(), rule_id, false, true);

        auto str_without_quotes = str.substr(1, str.size() - 2);
        container->addItem(
            isl::makeUnique<lexer::rule::Sequence>(text::removeEscaping(str_without_quotes, {})));

        rule_constructor.addLexerRule(str, std::move(container));
    }
}// namespace ccl::parser::reader::ast
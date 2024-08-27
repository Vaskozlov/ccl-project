#include <ccl/lexer/rule/container.hpp>
#include <ccl/lexer/rule/rule_reference.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_block.hpp>
#include <ccl/parser/rules_reader/rules_lexer.hpp>
#include <ccl/text/remove_escaping.hpp>

namespace ccl::parser::reader::ast
{
    using enum RulesLexerToken;

    static auto constructUnion(const lexer::Token &token) -> lexer::rule::RuleBlock
    {
        const auto repr = token.getRepr();
        auto [ascii_symbols, ranges] = lexer::rule::parseUnionDecl(repr);
        return std::make_unique<lexer::rule::Union>(isl::UtfSet(ascii_symbols, ranges));
    }

    static auto constructSequence(const lexer::Token &token) -> lexer::rule::RuleBlock
    {
        auto repr = token.getRepr();
        repr = repr.substr(1, repr.size() - 2);

        return std::make_unique<lexer::rule::Sequence>(text::removeEscaping(repr, {}));
    }

    static auto constructRuleReference(ParserBuilder &parser_builder, const lexer::Token &token)
        -> lexer::rule::RuleBlock
    {
        auto repr = token.getRepr();
        repr = repr.substr(1, repr.size() - 2);

        return std::make_unique<lexer::rule::RuleReference>(
            parser_builder.getLexicalAnalyzer(), text::removeEscaping(repr, {}));
    }

    static auto constructContainer(ParserBuilder &parser_builder, const parser::ast::Node *node)
        -> lexer::rule::RuleBlock
    {
        const auto *lexer_rule_alternative = static_cast<const RulesReaderNode *>(node);

        return isl::get<std::unique_ptr<lexer::rule::Container>>(
            lexer_rule_alternative->construct(parser_builder));
    }

    auto LexerRuleBlock::getValue() const -> const parser::ast::TokenNode *
    {
        const auto *value_node = front().get();
        return isl::as<const parser::ast::TokenNode *>(value_node);
    }

    auto LexerRuleBlock::getOptions() const -> const LexerRuleOptions *
    {
        const auto *value_node = back().get();
        return isl::as<const LexerRuleOptions *>(value_node);
    }

    auto LexerRuleBlock::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        using namespace lexer::rule;

        const auto *rule_block = static_cast<const parser::ast::TokenNode *>(front().get());
        auto resulted_block = RuleBlock{};

        switch (rule_block->getType()) {
        case ANGLE_OPEN:
            resulted_block = constructContainer(parser_builder, at(1).get());
            break;

        case UNION:
            resulted_block = constructUnion(rule_block->getToken());
            break;

        case STRING:
            resulted_block = constructSequence(rule_block->getToken());
            break;

        case RULE_REFERENCE:
            resulted_block = constructRuleReference(parser_builder, rule_block->getToken());
            break;

        default:
            isl::unreachable();
        }

        applyOptions(parser_builder, resulted_block.get());
        return isl::UniqueAny{std::move(resulted_block)};
    }

    auto LexerRuleBlock::applyOptions(
        ParserBuilder &parser_builder,
        lexer::rule::RuleBlockInterface *rule_block) const -> void
    {
        if (size() == 1) {
            return;
        }

        const auto *rule_option = static_cast<const LexerRuleOptions *>(back().get());
        const auto options = isl::get<std::vector<SmallId>>(rule_option->construct(parser_builder));

        // TODO: add support for custom repetition

        for (const auto option : options) {
            switch (option) {
            case STAR:
                rule_block->setClosure(lexer::rule::Closure{0, lexer::rule::Closure::max()});
                break;

            case PLUS:
                rule_block->setClosure(lexer::rule::Closure{1, lexer::rule::Closure::max()});
                break;

            case PREFIX_POSTFIX_OPERATOR:
                rule_block->setPrefix();
                break;

            case NOT_OPERATOR:
                rule_block->reverse();
                break;

            case HIDE_OPERATOR:
                rule_block->hideFromParser();
                break;

            default:
                isl::unreachable();
            }
        }
    }
}// namespace ccl::parser::reader::ast
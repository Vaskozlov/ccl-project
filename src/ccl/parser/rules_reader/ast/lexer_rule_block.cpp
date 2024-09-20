#include <ccl/lexer/rule/container.hpp>
#include <ccl/lexer/rule/rule_reference.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_block.hpp>
#include <ccl/parser/rules_reader/rules_lexer.hpp>
#include <ccl/text/remove_escaping.hpp>

namespace ccl::parser::reader::ast
{
    using enum RulesLexerToken;

    static auto constructUnion(const lexer::Token &token)
        -> std::unique_ptr<lexer::rule::RuleBlockInterface>
    {
        const auto repr = token.getRepr();
        auto [ascii_symbols, ranges] = lexer::rule::parseUnionDecl(repr);
        return std::make_unique<lexer::rule::Union>(isl::UtfSet(ascii_symbols, ranges));
    }

    static auto constructSequence(const lexer::Token &token)
        -> std::unique_ptr<lexer::rule::RuleBlockInterface>
    {
        auto repr = token.getRepr();
        repr = repr.substr(1, repr.size() - 2);

        return std::make_unique<lexer::rule::Sequence>(text::removeEscaping(repr, {}));
    }

    static auto constructRuleReference(ParserBuilder &parser_builder, const lexer::Token &token)
        -> std::unique_ptr<lexer::rule::RuleBlockInterface>
    {
        auto repr = token.getRepr();
        repr = repr.substr(1, repr.size() - 2);

        return std::make_unique<lexer::rule::RuleReference>(
            parser_builder.getLexicalAnalyzer(), text::removeEscaping(repr, {}));
    }

    static auto constructContainer(ParserBuilder &parser_builder, const parser::ast::Node *node)
        -> std::unique_ptr<lexer::rule::RuleBlockInterface>
    {
        const auto *lexer_rule_alternative = static_cast<const RulesReaderNode *>(node);

        return lexer_rule_alternative->construct(parser_builder).release<lexer::rule::Container>();
    }

    auto LexerRuleBlock::getValue() const -> const parser::ast::Terminal *
    {
        const auto *value_node = front().get();
        return isl::as<const parser::ast::Terminal *>(value_node);
    }

    auto LexerRuleBlock::getOptions() const -> const LexerRuleOptions *
    {
        const auto *value_node = back().get();
        return isl::as<const LexerRuleOptions *>(value_node);
    }

    auto LexerRuleBlock::construct(ParserBuilder &parser_builder) const -> isl::UniqueAny
    {
        using namespace lexer::rule;

        const auto *rule_block = static_cast<const parser::ast::Terminal *>(front().get());
        auto resulted_block = std::unique_ptr<RuleBlockInterface>{};

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
        auto rule_options = rule_option->construct(parser_builder);
        const auto options = rule_options.release<isl::SmallVector<SmallId, 4>>();

        // TODO: add support for custom repetition

        for (const auto option : *options) {
            switch (option) {
            case STAR:
                rule_block->setRepetition(lexer::rule::Repetition{0, lexer::rule::Repetition::max()});
                break;

            case PLUS:
                rule_block->setRepetition(lexer::rule::Repetition{1, lexer::rule::Repetition::max()});
                break;

            case EXTRACTABLE_MODIFIER:
                rule_block->makeExtractable();
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
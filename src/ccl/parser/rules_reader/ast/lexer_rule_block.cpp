#include <ccl/parser/rules_reader/ast/lexer_rule_block.hpp>

namespace ccl::parser::reader::ast
{
    static auto
        constructUnion(const lexer::Token &token) -> isl::UniquePtr<lexer::rule::RuleBlockInterface>
    {
        auto repr = token.getRepr();
        auto [ascii_symbols, ranges] = lexer::rule::parseUnionDecl(repr);
        return isl::makeUnique<lexer::rule::Union>(isl::UtfSet(ascii_symbols, ranges));
    }

    static auto constructSequence(const lexer::Token &token)
        -> isl::UniquePtr<lexer::rule::RuleBlockInterface>
    {
        auto repr = token.getRepr();
        repr = repr.substr(1, repr.size() - 2);
        return isl::makeUnique<lexer::rule::Sequence>(text::removeEscaping(repr, {}));
    }

    static auto constructRuleReference(
        RulesConstructor &rule_constructor,
        const lexer::Token &token) -> isl::UniquePtr<lexer::rule::RuleBlockInterface>
    {
        auto repr = token.getRepr();
        repr = repr.substr(1, repr.size() - 2);
        return isl::makeUnique<lexer::rule::RuleReference>(
            rule_constructor.getLexicalAnalyzer(), text::removeEscaping(repr, {}));
    }

    auto LexerRuleBlock::getValue() const -> const parser::ast::TokenNode *
    {
        const auto *value_node = this->front();
        return isl::as<const parser::ast::TokenNode *>(value_node);
    }

    auto LexerRuleBlock::getOptions() const -> const ast::LexerRuleOptions *
    {
        const auto *value_node = this->back();
        return isl::as<const ast::LexerRuleOptions *>(value_node);
    }

    auto LexerRuleBlock::construct(RulesConstructor &rule_constructor) const -> isl::UniqueAny
    {
        using namespace lexer::rule;

        const auto *rule_block = this->front();
        const auto *rule_block_as_token = isl::as<const parser::ast::TokenNode *>(rule_block);
        auto resulted_block = isl::UniquePtr<RuleBlockInterface>{};

        switch (rule_block_as_token->getType()) {
        case RulesLexerToken::UNION:
            resulted_block = constructUnion(rule_block_as_token->getToken());
            break;

        case RulesLexerToken::STRING:
            resulted_block = constructSequence(rule_block_as_token->getToken());
            break;

        case RulesLexerToken::RULE_REFERENCE:
            resulted_block =
                constructRuleReference(rule_constructor, rule_block_as_token->getToken());
            break;

        default:
            isl::unreachable();
        }

        if (this->size() == 2) {
            applyOptions(resulted_block.get());
        }

        return isl::UniqueAny{std::move(resulted_block)};
    }

    auto LexerRuleBlock::applyOptions(lexer::rule::RuleBlockInterface *rule_block) const -> void
    {
        const auto *last_node = this->back();
        const auto *rule_option = isl::as<const ast::LexerRuleOptions *>(last_node);

        for (auto option : rule_option->getOptions()) {
            switch (option) {
            case RulesLexerToken::STAR:
                rule_block->setClosure(lexer::rule::Closure{0, lexer::rule::Closure::max()});
                break;

            case RulesLexerToken::PLUS:
                rule_block->setClosure(lexer::rule::Closure{1, lexer::rule::Closure::max()});
                break;

            case RulesLexerToken::PREFIX_POSTFIX_OPERATOR:
                rule_block->setPrefix();
                break;

            case RulesLexerToken::NOT_OPERATOR:
                rule_block->reverse();
                break;

            case RulesLexerToken::HIDE_OPERATOR:
                rule_block->hideFromParser();
                break;

            default:
                isl::unreachable();
            }
        }
    }
}// namespace ccl::parser::reader::ast
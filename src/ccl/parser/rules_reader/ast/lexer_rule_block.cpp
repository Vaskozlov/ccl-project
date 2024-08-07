#include <ccl/lexer/rule/container.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_block.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_body.hpp>

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

    static auto constructContainer(
        RulesConstructor &rule_constructor,
        const parser::ast::Node *node) -> isl::UniquePtr<lexer::rule::RuleBlockInterface>
    {
        const auto *casted_node = dynamic_cast<const ast::LexerRuleBody *>(node);
        return casted_node->construct(rule_constructor)
            .get<isl::UniquePtr<lexer::rule::Container>>();
    }

    static auto constructNonContainerRule(
        RulesConstructor &rule_constructor,
        const lexer::Token &token) -> isl::UniquePtr<lexer::rule::RuleBlockInterface>
    {
        switch (token.getId()) {
        case RulesLexerToken::UNION:
            return constructUnion(token);

        case RulesLexerToken::STRING:
            return constructSequence(token);

        case RulesLexerToken::RULE_REFERENCE:
            return constructRuleReference(rule_constructor, token);

        default:
            isl::unreachable();
        }
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
        const auto *rule_block_as_token_node = isl::as<const parser::ast::TokenNode *>(rule_block);

        auto resulted_block = isl::UniquePtr<RuleBlockInterface>{};

        if (rule_block_as_token_node == nullptr) {
            resulted_block = constructContainer(rule_constructor, rule_block);
        } else {
            resulted_block =
                constructNonContainerRule(rule_constructor, rule_block_as_token_node->getToken());
        }

        applyOptions(rule_constructor, resulted_block.get());
        return isl::UniqueAny{std::move(resulted_block)};
    }

    auto LexerRuleBlock::applyOptions(
        RulesConstructor &rule_constructor,
        lexer::rule::RuleBlockInterface *rule_block) const -> void
    {
        if (this->size() == 1) {
            return;
        }

        const auto *last_node = this->back();
        const auto *rule_option = isl::as<const ast::LexerRuleOptions *>(last_node);
        auto options =
            isl::get<std::vector<RulesLexerToken>>(rule_option->construct(rule_constructor));

        for (auto option : options) {
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
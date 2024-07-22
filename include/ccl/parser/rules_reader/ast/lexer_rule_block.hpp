#ifndef CCL_PROJECT_LEXER_RULE_BLOCK_HPP
#define CCL_PROJECT_LEXER_RULE_BLOCK_HPP

#include <ccl/lexer/rule/rule_reference.hpp>
#include <ccl/lexer/rule/sequence.hpp>
#include <ccl/lexer/rule/union.hpp>
#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_options.hpp>
#include <ccl/parser/rules_reader/ast/rules_reader_node.hpp>
#include <ccl/text/remove_escaping.hpp>

namespace ccl::parser::reader::ast
{
    class LexerRuleBlock
      : public parser::ast::NodeSequence<isl::UniquePtr>
      , public RulesReaderNode
    {
    public:
        using parser::ast::NodeSequence<isl::UniquePtr>::NodeSequence;

        [[nodiscard]] auto getValue() const -> const parser::ast::TokenNode *
        {
            const auto *value_node = this->front();
            return isl::as<const parser::ast::TokenNode *>(value_node);
        }

        [[nodiscard]] auto getOptions() const -> const ast::LexerRuleOptions *
        {
            const auto *value_node = this->back();
            return isl::as<const ast::LexerRuleOptions *>(value_node);
        }

        auto construct(lexer::LexicalAnalyzer &lexical_analyzer) const -> isl::UniqueAny override
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
                    constructRuleReference(lexical_analyzer, rule_block_as_token->getToken());
                break;

            default:
                isl::unreachable();
            }

            if (this->size() == 2) {
                applyOptions(resulted_block.get());
            }

            return isl::UniqueAny{std::move(resulted_block)};
        }

        static auto constructUnion(const lexer::Token &token)
            -> isl::UniquePtr<lexer::rule::RuleBlockInterface>
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
            lexer::LexicalAnalyzer &lexical_analyzer,
            const lexer::Token &token) -> isl::UniquePtr<lexer::rule::RuleBlockInterface>
        {
            auto repr = token.getRepr();
            repr = repr.substr(1, repr.size() - 2);
            return isl::makeUnique<lexer::rule::RuleReference>(
                lexical_analyzer, text::removeEscaping(repr, {}));
        }

        auto applyOptions(lexer::rule::RuleBlockInterface *rule_block) const -> void
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
    };
}// namespace ccl::parser::reader::ast

#endif /* CCL_PROJECT_LEXER_RULE_BLOCK_HPP */

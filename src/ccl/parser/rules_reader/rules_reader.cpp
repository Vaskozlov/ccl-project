#include <ccl/parser/ast/token_node.hpp>
#include <ccl/parser/lr/glr_parser.hpp>
#include <ccl/parser/rules_reader/ast/any_block.hpp>
#include <ccl/parser/rules_reader/ast/lexer_block.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_alternative.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_block.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_body.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_decl.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_options.hpp>
#include <ccl/parser/rules_reader/ast/parser_block.hpp>
#include <ccl/parser/rules_reader/ast/parser_rule_alternatives.hpp>
#include <ccl/parser/rules_reader/ast/parser_rule_body.hpp>
#include <ccl/parser/rules_reader/ast/parser_rule_decl.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>

namespace ccl::parser::reader
{
    template<class T>
    static const auto DefaultNodeConstructor =
        [](Symbol production, std::vector<parser::ast::UnNodePtr> nodes) {
            return isl::makeUnique<T>(production, std::move(nodes));
        };

    static const auto DefaultAppender =
        [](Symbol /* unused */, std::vector<parser::ast::UnNodePtr> nodes) {
            auto rule_body = std::move(nodes.front());
            auto *casted_rule_body = isl::as<parser::ast::UnNodeSequence *>(rule_body.get());

            casted_rule_body->addNode(std::move(nodes.back()));
            return rule_body;
        };

    static const auto ReplaceUpperBlockWithLastElementFromCurrent =
        [](Symbol /* unused */, std::vector<parser::ast::UnNodePtr> nodes) {
            return std::move(nodes.back());
        };

    static const auto ConstructLexerGroup =
        [](Symbol production, std::vector<parser::ast::UnNodePtr> nodes) {
            auto result = isl::makeUnique<ast::LexerRuleBlock>(production);

            result->addNode(std::move(nodes.at(1)));

            if (nodes.size() == 4) {
                result->addNode(std::move(nodes.back()));
            }

            return result;
        };

    static const auto ConstructLexerRuleAlternative =
        [](Symbol production, std::vector<parser::ast::UnNodePtr> nodes) {
            auto lhs_node = std::move(nodes.front());
            auto rhs_node = std::move(nodes.back());

            auto alternative =
                isl::makeUnique<ast::LexerRuleAlternative>(RulesLexerToken::LEXER_RULE_ALTERNATIVE);

            alternative->addNode(std::move(lhs_node));
            alternative->addNode(std::move(rhs_node));

            auto result = DefaultNodeConstructor<ast::LexerRuleBody>(production, {});
            result->addNode(std::move(alternative));

            return result;
        };

    static const auto ConstructLexerRuleAlternativeWithAppend =
        [](Symbol production, std::vector<parser::ast::UnNodePtr> nodes) {
            auto lhs_node = std::move(nodes.at(1));
            auto rhs_node = std::move(nodes.back());

            auto result = isl::makeUnique<ast::LexerRuleAlternative>(production);

            result->addNode(std::move(lhs_node));
            result->addNode(std::move(rhs_node));

            auto front_node = std::move(nodes.front());

            nodes.clear();
            nodes.emplace_back(std::move(front_node));
            nodes.emplace_back(std::move(result));

            return DefaultAppender(production, std::move(nodes));
        };

    static const auto ConstructParserRuleAlternative =
        [](Symbol /* unused */, std::vector<parser::ast::UnNodePtr> nodes)
        -> parser::ast::UnNodePtr {
        auto first_node = std::move(nodes.front());

        auto *first_node_as_rule_alternative =
            isl::as<ast::ParserRuleAlternatives *>(first_node.get());

        auto rhs_rule_body =
            isl::makeUnique<ast::ParserRuleBody>(RulesLexerToken::PARSER_RULE_BODY);

        rhs_rule_body->addNode(std::move(nodes.back()));

        if (first_node_as_rule_alternative == nullptr) {
            auto result = isl::makeUnique<ast::ParserRuleAlternatives>(PARSER_RULE_ALTERNATIVE);

            result->addNode(std::move(first_node));
            result->addNode(std::move(rhs_rule_body));

            return result;
        }

        first_node_as_rule_alternative->addNode(std::move(rhs_rule_body));

        return first_node;
    };

    template<class T>
    static const auto AppendToTheLastNodeIfTElseDefaultAppend =
        [](ccl::parser::Symbol production, std::vector<ccl::parser::ast::UnNodePtr> nodes) {
            auto &rule_body = nodes.front();
            auto *casted_rule_body = isl::as<T *>(rule_body.get());

            if (casted_rule_body == nullptr) {
                return DefaultAppender(production, std::move(nodes));
            }

            auto *casted_last_node =
                isl::as<parser::ast::UnNodeSequence *>(casted_rule_body->back());

            casted_last_node->addNode(std::move(nodes.back()));

            return std::move(rule_body);
        };


    using Rule = ccl::parser::Rule;

    static const GrammarStorage RulesGrammar{
        true,
        RulesLexerToken::EPSILON,
        {
            {
                RulesLexerToken::GOAL,
                {
                    Rule{
                        {
                            RulesLexerToken::ANY_BLOCK,
                        },
                    },
                },
            },
            {
                RulesLexerToken::ANY_BLOCK,
                {
                    Rule{
                        {
                            RulesLexerToken::LEXER_BLOCK_DEFINITION,
                        },
                        DefaultNodeConstructor<ast::AnyBlock>,
                    },
                    Rule{
                        {
                            RulesLexerToken::PARSER_BLOCK_DEFINITION,
                        },
                        DefaultNodeConstructor<ast::AnyBlock>,
                    },

                    Rule{
                        {
                            RulesLexerToken::ANY_BLOCK,
                            RulesLexerToken::LEXER_BLOCK_DEFINITION,
                        },
                        DefaultAppender,
                    },
                    Rule{
                        {
                            RulesLexerToken::ANY_BLOCK,
                            RulesLexerToken::PARSER_BLOCK_DEFINITION,
                        },
                        DefaultAppender,
                    },
                },
            },
            {
                RulesLexerToken::LEXER_BLOCK_DEFINITION,
                {
                    Rule{
                        {
                            RulesLexerToken::LEXER_START,
                            RulesLexerToken::LEXER_BLOCK,
                        },
                        ReplaceUpperBlockWithLastElementFromCurrent,
                    },
                },
            },
            {
                RulesLexerToken::LEXER_BLOCK,
                {
                    Rule{
                        {
                            RulesLexerToken::LEXER_RULE_DECL,
                        },
                        DefaultNodeConstructor<ast::LexerBlock>,
                    },
                    Rule{
                        {
                            RulesLexerToken::LEXER_BLOCK,
                            RulesLexerToken::LEXER_RULE_DECL,
                        },
                        DefaultAppender,
                    },
                },
            },
            {
                RulesLexerToken::LEXER_RULE_DECL,
                {
                    Rule{
                        {
                            RulesLexerToken::RULE_IDENTIFIER,
                            RulesLexerToken::LEXER_RULE_BODY,
                        },
                        DefaultNodeConstructor<ast::LexerRuleDecl>,
                    },
                    Rule{
                        {
                            RulesLexerToken::RULE_IDENTIFIER,
                            RulesLexerToken::EXCLAMATION_POINT,
                            RulesLexerToken::LEXER_RULE_BODY,
                        },
                        DefaultNodeConstructor<ast::LexerRuleDecl>,
                    },
                },
            },
            {
                RulesLexerToken::LEXER_RULE_BODY,
                {
                    Rule{
                        {
                            RulesLexerToken::LEXER_RULE_BLOCK,
                        },
                        DefaultNodeConstructor<ast::LexerRuleBody>,
                    },
                    Rule{
                        {
                            RulesLexerToken::LEXER_RULE_BODY,
                            RulesLexerToken::LEXER_RULE_BLOCK,
                        },
                        DefaultAppender,
                    },
                    Rule{
                        {
                            RulesLexerToken::LEXER_RULE_BODY,
                            RulesLexerToken::OR,
                            RulesLexerToken::LEXER_RULE_BLOCK,
                        },
                        ConstructLexerRuleAlternative,
                    },
                },
            },
            {
                RulesLexerToken::LEXER_RULE_BLOCK,
                {
                    Rule{
                        {
                            RulesLexerToken::UNION,
                            RulesLexerToken::LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleBlock>,
                    },
                    Rule{
                        {
                            RulesLexerToken::STRING,
                            RulesLexerToken::LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleBlock>,
                    },
                    Rule{
                        {
                            RulesLexerToken::RULE_REFERENCE,
                            RulesLexerToken::LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleBlock>,
                    },
                    Rule{
                        {
                            RulesLexerToken::ANGLE_OPEN,
                            RulesLexerToken::LEXER_RULE_BODY,
                            RulesLexerToken::ANGLE_CLOSE,
                            RulesLexerToken::LEXER_RULE_OPTIONS,
                        },
                        ConstructLexerGroup,
                    },
                },
            },
            {
                RulesLexerToken::LEXER_RULE_OPTIONS,
                {
                    Rule{
                        {
                            RulesLexerToken::EPSILON,
                        },
                        DefaultNodeConstructor<ast::LexerRuleOptions>,
                    },
                    Rule{
                        {
                            RulesLexerToken::PLUS,
                            RulesLexerToken::LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleOptions>,
                    },
                    Rule{
                        {
                            RulesLexerToken::STAR,
                            RulesLexerToken::LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleOptions>,
                    },
                    Rule{
                        {
                            RulesLexerToken::PREFIX_POSTFIX_OPERATOR,
                            RulesLexerToken::LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleOptions>,
                    },
                    Rule{
                        {
                            RulesLexerToken::HIDE_OPERATOR,
                            RulesLexerToken::LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleOptions>,
                    },
                    Rule{
                        {
                            RulesLexerToken::NOT_OPERATOR,
                            RulesLexerToken::LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleOptions>,
                    },
                    Rule{
                        {
                            RulesLexerToken::CURLY_OPEN,
                            RulesLexerToken::NUMBER,
                            RulesLexerToken::CURLY_CLOSE,
                            RulesLexerToken::LEXER_RULE_OPTIONS,
                        },
                    },
                    Rule{
                        {
                            RulesLexerToken::CURLY_OPEN,
                            RulesLexerToken::NUMBER,
                            RulesLexerToken::COMMA,
                            RulesLexerToken::NUMBER,
                            RulesLexerToken::CURLY_CLOSE,
                            RulesLexerToken::LEXER_RULE_OPTIONS,
                        },
                    },
                },
            },
            {
                RulesLexerToken::PARSER_BLOCK_DEFINITION,
                {
                    Rule{
                        {
                            RulesLexerToken::PARSER_START,
                            RulesLexerToken::PARSER_BLOCK,
                        },
                        ReplaceUpperBlockWithLastElementFromCurrent,
                    },
                },
            },
            {
                RulesLexerToken::PARSER_BLOCK,
                {
                    Rule{
                        {
                            RulesLexerToken::PARSER_RULE_DECL,
                        },
                        DefaultNodeConstructor<ast::ParserBlock>,
                    },
                    Rule{
                        {
                            RulesLexerToken::PARSER_BLOCK,
                            RulesLexerToken::PARSER_RULE_DECL,
                        },
                        DefaultAppender,
                    },
                },
            },
            {
                RulesLexerToken::PARSER_RULE_DECL,
                {
                    Rule{
                        {
                            RulesLexerToken::RULE_IDENTIFIER,
                            RulesLexerToken::PARSER_RULE_BODY,
                        },
                        DefaultNodeConstructor<ast::ParserRuleDecl>,
                    },
                },
            },
            {
                RulesLexerToken::PARSER_RULE_BODY,
                {
                    Rule{
                        {
                            RulesLexerToken::IDENTIFIER,
                        },
                        DefaultNodeConstructor<ast::ParserRuleBody>,
                    },
                    Rule{
                        {
                            RulesLexerToken::STRING,
                        },
                        DefaultNodeConstructor<ast::ParserRuleBody>,
                    },
                    Rule{
                        {
                            RulesLexerToken::ANY_PLACE_STRING,
                        },
                        DefaultNodeConstructor<ast::ParserRuleBody>,
                    },
                    Rule{
                        {
                            RulesLexerToken::PARSER_RULE_BODY,
                            RulesLexerToken::IDENTIFIER,
                        },
                        AppendToTheLastNodeIfTElseDefaultAppend<ast::ParserRuleAlternatives>,
                    },
                    Rule{
                        {
                            RulesLexerToken::PARSER_RULE_BODY,
                            RulesLexerToken::STRING,
                        },
                        AppendToTheLastNodeIfTElseDefaultAppend<ast::ParserRuleAlternatives>,
                    },
                    Rule{
                        {
                            RulesLexerToken::PARSER_RULE_BODY,
                            RulesLexerToken::ANY_PLACE_STRING,
                        },
                        AppendToTheLastNodeIfTElseDefaultAppend<ast::ParserRuleAlternatives>,
                    },
                    Rule{
                        {
                            RulesLexerToken::PARSER_RULE_BODY,
                            RulesLexerToken::OR,
                            RulesLexerToken::IDENTIFIER,
                        },
                        ConstructParserRuleAlternative,
                    },
                    Rule{
                        {
                            RulesLexerToken::PARSER_RULE_BODY,
                            RulesLexerToken::OR,
                            RulesLexerToken::STRING,
                        },
                        ConstructParserRuleAlternative,
                    },
                    Rule{
                        {
                            RulesLexerToken::PARSER_RULE_BODY,
                            RulesLexerToken::OR,
                            RulesLexerToken::ANY_PLACE_STRING,
                        },
                        ConstructParserRuleAlternative,
                    },
                },
            },
        },
    };

    RulesReader::RulesReader(isl::string_view input, isl::string_view filename)
      : readerOptions{
            ReaderOption::WARNINGS_ENABLE,
            ReaderOption::SUGGESTIONS_ENABLE,
        }
    {
        static auto initial_rule = Rule{{RulesLexerToken::ANY_BLOCK}};

        auto start_item =
            LrItem{std::addressof(initial_rule), 0, RulesLexerToken::GOAL, RulesLexerToken::EOI};

        auto tokenizer = RulesLexer.getTokenizer(input, filename);
        auto lr_parser =
            LrParser(start_item, RulesLexerToken::EPSILON, RulesGrammar, [](auto elem) {
                return std::string{ToStringRulesLexerToken.at(elem)};
            });

        auto node = lr_parser.parse(tokenizer);
        dynamic_cast<ast::RulesReaderNode *>(node.get())->construct(rulesConstructor);
    }

    auto RulesReader::getParserBuilder() -> ParserBuilder &
    {
        return rulesConstructor;
    }
}// namespace ccl::parser::reader
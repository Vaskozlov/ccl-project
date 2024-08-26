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
        [](Symbol production, std::vector<parser::ast::SharedNode<>> nodes) {
            return parser::ast::SharedNode<T>{production, std::move(nodes)};
        };

    static const auto DefaultAppender =
        [](Symbol /* unused */, std::vector<parser::ast::SharedNode<>> nodes) {
            auto rule_body = nodes.front();
            auto casted_rule_body = isl::dynamicPointerCast<parser::ast::NodeOfNodes>(rule_body);

            // possible move
            casted_rule_body->addNode(nodes.back());

            return casted_rule_body;
        };

    static const auto ReplaceUpperBlockWithLastElementFromCurrent =
        [](Symbol /* unused */, std::vector<parser::ast::SharedNode<>> nodes) {
            return isl::dynamicPointerCast<parser::ast::NodeOfNodes>(nodes.back());
        };

    static const auto ConstructLexerGroup =
        [](Symbol production, std::vector<parser::ast::SharedNode<>> nodes) {
            auto result = parser::ast::SharedNode<ast::LexerRuleBlock>(production);
            result->addNode(nodes.at(1));

            if (nodes.size() == 4) {
                result->addNode(nodes.back());
            }

            return result;
        };

    static const auto ConstructLexerRuleAlternative =
        [](Symbol production, std::vector<parser::ast::SharedNode<>> nodes) {
            auto lhs_node = nodes.front();
            auto rhs_node = nodes.back();

            auto alternative =
                parser::ast::SharedNode<ast::LexerRuleAlternative>(LEXER_RULE_ALTERNATIVE);

            alternative->addNode(lhs_node);
            alternative->addNode(rhs_node);

            auto result = parser::ast::SharedNode<ast::LexerRuleBody>(production);
            result->addNode(parser::ast::SharedNode<>{std::move(alternative)});

            return result;
        };

    static const auto ConstructLexerRuleAlternativeWithAppend =
        [](Symbol production, std::vector<parser::ast::SharedNode<>> nodes) {
            auto lhs_node = nodes.at(1);
            auto rhs_node = nodes.back();

            auto result = parser::ast::SharedNode<ast::LexerRuleAlternative>(production);

            result->addNode(lhs_node);
            result->addNode(rhs_node);

            auto front_node = nodes.front();

            nodes.clear();
            nodes.emplace_back(std::move(front_node));
            nodes.emplace_back(std::move(result));

            return DefaultAppender(production, std::move(nodes));
        };

    static const auto ConstructParserRuleAlternative =
        [](Symbol /* unused */, std::vector<parser::ast::SharedNode<>> nodes)
        -> parser::ast::SharedNode<parser::ast::NodeOfNodes> {
        auto first_node = nodes.front();

        auto first_node_as_rule_alternative =
            isl::dynamicPointerCast<ast::ParserRuleAlternatives>(first_node);

        auto rhs_rule_body = parser::ast::SharedNode<ast::ParserRuleBody>(PARSER_RULE_BODY);

        rhs_rule_body->addNode(nodes.back());

        if (first_node_as_rule_alternative == nullptr) {
            auto result =
                parser::ast::SharedNode<ast::ParserRuleAlternatives>(PARSER_RULE_ALTERNATIVE);

            result->addNode(first_node);
            result->addNode(rhs_rule_body);

            return result;
        }

        first_node_as_rule_alternative->addNode(rhs_rule_body);
        return first_node_as_rule_alternative;
    };

    template<class T>
    static const auto AppendToTheLastNodeIfTElseDefaultAppend =
        [](Symbol production, std::vector<parser::ast::SharedNode<>> nodes)
        -> parser::ast::SharedNode<parser::ast::NodeOfNodes> {
        auto rule_body = nodes.front();
        auto casted_rule_body = isl::dynamicPointerCast<T>(rule_body);

        if (casted_rule_body == nullptr) {
            return DefaultAppender(production, std::move(nodes));
        }

        auto casted_last_node =
            isl::dynamicPointerCast<parser::ast::NodeOfNodes>(casted_rule_body->back());

        casted_last_node->addNode(std::move(nodes.back()));
        return casted_rule_body;
    };

    static const GrammarStorage RulesGrammar{
        true,
        EPSILON,
        {
            {
                GOAL,
                {
                    Rule{
                        {
                            ANY_BLOCK,
                        },
                    },
                },
            },
            {
                ANY_BLOCK,
                {
                    Rule{
                        {
                            LEXER_BLOCK_DEFINITION,
                        },
                        DefaultNodeConstructor<ast::AnyBlock>,
                    },
                    Rule{
                        {
                            PARSER_BLOCK_DEFINITION,
                        },
                        DefaultNodeConstructor<ast::AnyBlock>,
                    },

                    Rule{
                        {
                            ANY_BLOCK,
                            LEXER_BLOCK_DEFINITION,
                        },
                        DefaultAppender,
                    },
                    Rule{
                        {
                            ANY_BLOCK,
                            PARSER_BLOCK_DEFINITION,
                        },
                        DefaultAppender,
                    },
                },
            },
            {
                LEXER_BLOCK_DEFINITION,
                {
                    Rule{
                        {
                            LEXER_START,
                            LEXER_BLOCK,
                        },
                        ReplaceUpperBlockWithLastElementFromCurrent,
                    },
                },
            },
            {
                LEXER_BLOCK,
                {
                    Rule{
                        {
                            LEXER_RULE_DECL,
                        },
                        DefaultNodeConstructor<ast::LexerBlock>,
                    },
                    Rule{
                        {
                            LEXER_BLOCK,
                            LEXER_RULE_DECL,
                        },
                        DefaultAppender,
                    },
                },
            },
            {
                LEXER_RULE_DECL,
                {
                    Rule{
                        {
                            RULE_IDENTIFIER,
                            LEXER_RULE_BODY,
                        },
                        DefaultNodeConstructor<ast::LexerRuleDecl>,
                    },
                    Rule{
                        {
                            RULE_IDENTIFIER,
                            EXCLAMATION_POINT,
                            LEXER_RULE_BODY,
                        },
                        DefaultNodeConstructor<ast::LexerRuleDecl>,
                    },
                },
            },
            {
                LEXER_RULE_BODY,
                {
                    Rule{
                        {
                            LEXER_RULE_BLOCK,
                        },
                        DefaultNodeConstructor<ast::LexerRuleBody>,
                    },
                    Rule{
                        {
                            LEXER_RULE_BODY,
                            LEXER_RULE_BLOCK,
                        },
                        DefaultAppender,
                    },
                    Rule{
                        {
                            LEXER_RULE_BODY,
                            OR,
                            LEXER_RULE_BLOCK,
                        },
                        ConstructLexerRuleAlternative,
                    },
                },
            },
            {
                LEXER_RULE_BLOCK,
                {
                    Rule{
                        {
                            UNION,
                            LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleBlock>,
                    },
                    Rule{
                        {
                            STRING,
                            LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleBlock>,
                    },
                    Rule{
                        {
                            RULE_REFERENCE,
                            LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleBlock>,
                    },
                    Rule{
                        {
                            ANGLE_OPEN,
                            LEXER_RULE_BODY,
                            ANGLE_CLOSE,
                            LEXER_RULE_OPTIONS,
                        },
                        ConstructLexerGroup,
                    },
                },
            },
            {
                LEXER_RULE_OPTIONS,
                {
                    Rule{
                        {
                            EPSILON,
                        },
                        DefaultNodeConstructor<ast::LexerRuleOptions>,
                    },
                    Rule{
                        {
                            PLUS,
                            LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleOptions>,
                    },
                    Rule{
                        {
                            STAR,
                            LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleOptions>,
                    },
                    Rule{
                        {
                            PREFIX_POSTFIX_OPERATOR,
                            LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleOptions>,
                    },
                    Rule{
                        {
                            HIDE_OPERATOR,
                            LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleOptions>,
                    },
                    Rule{
                        {
                            NOT_OPERATOR,
                            LEXER_RULE_OPTIONS,
                        },
                        DefaultNodeConstructor<ast::LexerRuleOptions>,
                    },
                    Rule{
                        {
                            CURLY_OPEN,
                            NUMBER,
                            CURLY_CLOSE,
                            LEXER_RULE_OPTIONS,
                        },
                    },
                    Rule{
                        {
                            CURLY_OPEN,
                            NUMBER,
                            COMMA,
                            NUMBER,
                            CURLY_CLOSE,
                            LEXER_RULE_OPTIONS,
                        },
                    },
                },
            },
            {
                PARSER_BLOCK_DEFINITION,
                {
                    Rule{
                        {
                            PARSER_START,
                            PARSER_BLOCK,
                        },
                        ReplaceUpperBlockWithLastElementFromCurrent,
                    },
                },
            },
            {
                PARSER_BLOCK,
                {
                    Rule{
                        {
                            PARSER_RULE_DECL,
                        },
                        DefaultNodeConstructor<ast::ParserBlock>,
                    },
                    Rule{
                        {
                            PARSER_BLOCK,
                            PARSER_RULE_DECL,
                        },
                        DefaultAppender,
                    },
                },
            },
            {
                PARSER_RULE_DECL,
                {
                    Rule{
                        {
                            RULE_IDENTIFIER,
                            PARSER_RULE_BODY,
                        },
                        DefaultNodeConstructor<ast::ParserRuleDecl>,
                    },
                },
            },
            {
                PARSER_RULE_BODY,
                {
                    Rule{
                        {
                            IDENTIFIER,
                        },
                        DefaultNodeConstructor<ast::ParserRuleBody>,
                    },
                    Rule{
                        {
                            STRING,
                        },
                        DefaultNodeConstructor<ast::ParserRuleBody>,
                    },
                    Rule{
                        {
                            ANY_PLACE_STRING,
                        },
                        DefaultNodeConstructor<ast::ParserRuleBody>,
                    },
                    Rule{
                        {
                            PARSER_RULE_BODY,
                            IDENTIFIER,
                        },
                        AppendToTheLastNodeIfTElseDefaultAppend<ast::ParserRuleAlternatives>,
                    },
                    Rule{
                        {
                            PARSER_RULE_BODY,
                            STRING,
                        },
                        AppendToTheLastNodeIfTElseDefaultAppend<ast::ParserRuleAlternatives>,
                    },
                    Rule{
                        {
                            PARSER_RULE_BODY,
                            ANY_PLACE_STRING,
                        },
                        AppendToTheLastNodeIfTElseDefaultAppend<ast::ParserRuleAlternatives>,
                    },
                    Rule{
                        {
                            PARSER_RULE_BODY,
                            OR,
                            IDENTIFIER,
                        },
                        ConstructParserRuleAlternative,
                    },
                    Rule{
                        {
                            PARSER_RULE_BODY,
                            OR,
                            STRING,
                        },
                        ConstructParserRuleAlternative,
                    },
                    Rule{
                        {
                            PARSER_RULE_BODY,
                            OR,
                            ANY_PLACE_STRING,
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
        static auto initial_rule = Rule{{ANY_BLOCK}};

        const auto start_item = LrItem{
            .dottedRule =
                {
                    .rule = std::addressof(initial_rule),
                    .dotPosition = 0,
                },
            .production = GOAL,
            .lookAhead = EOI,
        };

        auto tokenizer = RulesLexer.getTokenizer(input, filename);

        const auto lr_parser = LrParser(start_item, EPSILON, RulesGrammar, [](auto elem) {
            return std::string{ToStringRulesLexerToken.at(elem)};
        });

        const auto [root_node, algorithm] = lr_parser.parse(tokenizer);
        dynamic_cast<const ast::RulesReaderNode *>(root_node.get())->construct(rulesConstructor);
    }

    auto RulesReader::getParserBuilder() -> ParserBuilder &
    {
        return rulesConstructor;
    }
}// namespace ccl::parser::reader

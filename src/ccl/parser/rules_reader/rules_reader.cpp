#include <ccl/parser/dot/dot_repr.hpp>
#include <ccl/parser/rules_reader/ast/blocks.hpp>
#include <ccl/parser/rules_reader/ast/lexer_block.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_alternative.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_block.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_options.hpp>
#include <ccl/parser/rules_reader/ast/parser_block.hpp>
#include <ccl/parser/rules_reader/ast/parser_rule.hpp>
#include <ccl/parser/rules_reader/rules_lexer.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>
#include <ccll/ccll.hpp>
#include <isl/io.hpp>

namespace ccl::parser::reader
{
    static const parser::ast::Node::ConversionTable ConversionTable{
        {
            BLOCKS,
            parser::ast::NodeOfNodes::reconstructNode<ast::Blocks>,
        },
        {
            LEXER_BLOCK,
            parser::ast::NodeOfNodes::reconstructNode<ast::LexerBlock>,
        },
        {
            LEXER_RULE_ALTERNATIVE,
            parser::ast::NodeOfNodes::reconstructNode<ast::LexerRuleAlternative>,
        },
        {
            LEXER_RULE,
            parser::ast::NodeOfNodes::reconstructNode<ast::LexerRule>,
        },
        {
            LEXER_RULE_BLOCK,
            parser::ast::NodeOfNodes::reconstructNode<ast::LexerRuleBlock>,
        },
        {
            LEXER_RULE_OPTIONS,
            parser::ast::NodeOfNodes::reconstructNode<ast::LexerRuleOptions>,
        },
        {
            PARSER_BLOCK,
            parser::ast::NodeOfNodes::reconstructNode<ast::ParserBlock>,
        },
        {
            PARSER_RULE_ALTERNATIVE,
            parser::ast::NodeOfNodes::reconstructNode<ast::ParserRuleAlternatives>,
        },
        {
            PARSER_RULE,
            parser::ast::NodeOfNodes::reconstructNode<ast::ParserRule>,
        },
    };

    // NOLINTNEXTLINE
    static const GrammarStorage RulesGrammar{
        true,
        EPSILON,
        GOAL,
        {
            {
                GOAL,
                {
                    Rule{
                        {BLOCKS},
                    },
                },
            },
            {
                BLOCKS,
                {
                    Rule{
                        {EPSILON},
                    },
                    Rule{
                        {LEXER_START, LEXER_BLOCK, BLOCKS},
                    },
                    Rule{
                        {PARSER_START, PARSER_BLOCK, BLOCKS},
                    },
                },
            },
            {
                LEXER_BLOCK,
                {
                    Rule{
                        {EPSILON},
                    },
                    Rule{
                        {
                            RULE_IDENTIFIER,
                            LEXER_RULE_ALTERNATIVE,
                            LEXER_BLOCK,
                        },
                    },
                    Rule{
                        {
                            RULE_IDENTIFIER,
                            EXCLAMATION_POINT,
                            LEXER_RULE_ALTERNATIVE,
                            LEXER_BLOCK,
                        },
                    },
                },
            },
            {
                LEXER_RULE_ALTERNATIVE,
                {
                    Rule{
                        {LEXER_RULE_ALTERNATIVE, OR, LEXER_RULE},
                    },
                    Rule{
                        {LEXER_RULE},
                    },
                },
            },
            {
                LEXER_RULE,
                {
                    Rule{
                        {LEXER_RULE, LEXER_RULE_BLOCK},
                    },
                    Rule{{EPSILON}},
                },
            },
            {
                LEXER_RULE_BLOCK,
                {
                    Rule{
                        {UNION, LEXER_RULE_OPTIONS},
                    },
                    Rule{
                        {STRING, LEXER_RULE_OPTIONS},
                    },
                    Rule{
                        {RULE_REFERENCE, LEXER_RULE_OPTIONS},
                    },
                    Rule{
                        {
                            ANGLE_OPEN,
                            LEXER_RULE_ALTERNATIVE,
                            ANGLE_CLOSE,
                            LEXER_RULE_OPTIONS,
                        },
                    },
                },
            },
            {
                LEXER_RULE_OPTIONS,
                {
                    Rule{
                        {EPSILON},
                    },
                    Rule{
                        {PLUS, LEXER_RULE_OPTIONS},
                    },
                    Rule{
                        {STAR, LEXER_RULE_OPTIONS},
                    },
                    Rule{
                        {PREFIX_POSTFIX_OPERATOR, LEXER_RULE_OPTIONS},
                    },
                    Rule{
                        {HIDE_OPERATOR, LEXER_RULE_OPTIONS},
                    },
                    Rule{
                        {NOT_OPERATOR, LEXER_RULE_OPTIONS},
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
                PARSER_BLOCK,
                {
                    Rule{
                        {EPSILON},
                    },
                    Rule{
                        {RULE_IDENTIFIER, PARSER_RULE_ALTERNATIVE, PARSER_BLOCK},
                    },
                },
            },
            {
                PARSER_RULE_ALTERNATIVE,
                {
                    Rule{
                        {PARSER_RULE_ALTERNATIVE, OR, PARSER_RULE},
                    },
                    Rule{
                        {PARSER_RULE},
                    },
                },
            },
            {
                PARSER_RULE,
                {
                    Rule{
                        {IDENTIFIER},
                    },
                    Rule{
                        {STRING},
                    },
                    Rule{
                        {ANY_PLACE_STRING},
                    },
                    Rule{
                        {IDENTIFIER, PARSER_RULE},
                    },
                    Rule{
                        {STRING, PARSER_RULE},
                    },
                    Rule{
                        {ANY_PLACE_STRING, PARSER_RULE},
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
        auto initial_rule = Rule{{BLOCKS}};

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

        auto id_to_str = [](auto elem) {
            return std::string{ToStringRulesLexerToken.at(elem)};
        };

        const auto gll_parser = LrParser(start_item, EPSILON, RulesGrammar, id_to_str);
        const auto [root, algorithm] = gll_parser.parse(tokenizer);

        // if (roots.size() != 1) {
            // throw std::runtime_error{"Unable to construct tree"};
        // }

        // auto root = isl::staticPointerCast<parser::ast::NodeOfNodes>(std::move(roots.front()));
        parser::ast::Node::cast(ConversionTable, root.get());

        static_cast<ast::RulesReaderNode *>(root.get())->construct(rulesConstructor);
    }

    auto RulesReader::getParserBuilder() -> ParserBuilder &
    {
        return rulesConstructor;
    }
}// namespace ccl::parser::reader

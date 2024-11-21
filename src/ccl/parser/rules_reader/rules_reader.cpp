#include <ccl/parser/dot/dot_repr.hpp>
#include <ccl/parser/ll/gll_parser.hpp>
#include <ccl/parser/rules_reader/ast/blocks.hpp>
#include <ccl/parser/rules_reader/ast/lexer_block.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_alternative.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_block.hpp>
#include <ccl/parser/rules_reader/ast/lexer_rule_options.hpp>
#include <ccl/parser/rules_reader/ast/parser_block.hpp>
#include <ccl/parser/rules_reader/ast/parser_rule.hpp>
#include <ccl/parser/rules_reader/ast/parser_rule_alternatives.hpp>
#include <ccl/parser/rules_reader/rules_lexer.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>
#include <ccll/ccll.hpp>
#include <isl/io.hpp>

namespace ccl::parser::reader
{
    template<typename T>
    static void (*ReconstructorPtr)(ccl::parser::ast::NonTerminal *) =
        ccl::parser::ast::NonTerminal::reconstructNode<T>;

    static auto getConversionTable() -> const parser::ast::Node::ConversionTable &
    {
        const static parser::ast::Node::ConversionTable conversion_table{
            {
                BLOCKS,
                ReconstructorPtr<ast::Blocks>,
            },
            {
                LEXER_BLOCK,
                ReconstructorPtr<ast::LexerBlock>,
            },
            {
                LEXER_RULE_ALTERNATIVE,
                ReconstructorPtr<ast::LexerRuleAlternative>,
            },
            {
                LEXER_RULE,
                ReconstructorPtr<ast::LexerRule>,
            },
            {
                LEXER_RULE_BLOCK,
                ReconstructorPtr<ast::LexerRuleBlock>,
            },
            {
                LEXER_RULE_OPTIONS,
                ReconstructorPtr<ast::LexerRuleOptions>,
            },
            {
                PARSER_BLOCK,
                ReconstructorPtr<ast::ParserBlock>,
            },
            {
                PARSER_RULE_ALTERNATIVE,
                ReconstructorPtr<ast::ParserRuleAlternatives>,
            },
            {
                PARSER_RULE,
                ReconstructorPtr<ast::ParserRule>,
            },
        };

        return conversion_table;
    }

    static auto getRulesGrammar() -> const GrammarStorage &
    {
        // NOLINTNEXTLINE
        const static GrammarStorage rules_grammar{
            true,
            EPSILON,
            GOAL,
            {
                {
                    GOAL,
                    {
                        Rule{
                            BLOCKS,
                        },
                    },
                },
                {
                    BLOCKS,
                    {
                        Rule{
                            EPSILON,
                        },
                        Rule{
                            LEXER_START,
                            LEXER_BLOCK,
                            BLOCKS,
                        },
                        Rule{
                            PARSER_START,
                            PARSER_BLOCK,
                            BLOCKS,
                        },
                    },
                },
                {
                    LEXER_BLOCK,
                    {
                        Rule{
                            EPSILON,
                        },
                        Rule{
                            RULE_IDENTIFIER,
                            LEXER_RULE_ALTERNATIVE,
                            LEXER_BLOCK,
                        },
                        Rule{
                            RULE_IDENTIFIER,
                            EXCLAMATION_POINT,
                            LEXER_RULE_ALTERNATIVE,
                            LEXER_BLOCK,
                        },
                    },
                },
                {
                    LEXER_RULE_ALTERNATIVE,
                    {
                        Rule{
                            LEXER_RULE_ALTERNATIVE,
                            OR,
                            LEXER_RULE,
                        },
                        Rule{
                            LEXER_RULE,
                        },
                    },
                },
                {
                    LEXER_RULE,
                    {
                        Rule{
                            LEXER_RULE,
                            LEXER_RULE_BLOCK,
                        },
                        Rule{
                            EPSILON,
                        },
                    },
                },
                {
                    LEXER_RULE_BLOCK,
                    {
                        Rule{
                            UNION,
                            LEXER_RULE_OPTIONS,
                        },
                        Rule{
                            STRING,
                            LEXER_RULE_OPTIONS,
                        },
                        Rule{
                            RULE_REFERENCE,
                            LEXER_RULE_OPTIONS,
                        },
                        Rule{
                            ANGLE_OPEN,
                            LEXER_RULE_ALTERNATIVE,
                            ANGLE_CLOSE,
                            LEXER_RULE_OPTIONS,
                        },
                    },
                },
                {
                    LEXER_RULE_OPTIONS,
                    {
                        Rule{
                            EPSILON,
                        },
                        Rule{
                            PLUS,
                            LEXER_RULE_OPTIONS,
                        },
                        Rule{
                            STAR,
                            LEXER_RULE_OPTIONS,
                        },
                        Rule{
                            EXTRACTABLE_MODIFIER,
                            LEXER_RULE_OPTIONS,
                        },
                        Rule{
                            HIDE_OPERATOR,
                            LEXER_RULE_OPTIONS,
                        },
                        Rule{
                            NOT_OPERATOR,
                            LEXER_RULE_OPTIONS,
                        },
                        Rule{
                            CURLY_OPEN,
                            NUMBER,
                            CURLY_CLOSE,
                            LEXER_RULE_OPTIONS,
                        },
                        Rule{
                            CURLY_OPEN,
                            NUMBER,
                            COMMA,
                            NUMBER,
                            CURLY_CLOSE,
                            LEXER_RULE_OPTIONS,
                        },
                    },
                },
                {
                    PARSER_BLOCK,
                    {
                        Rule{
                            EPSILON,
                        },
                        Rule{
                            RULE_IDENTIFIER,
                            PARSER_RULE_ALTERNATIVE,
                            PARSER_BLOCK,
                        },
                    },
                },
                {
                    PARSER_RULE_ALTERNATIVE,
                    {
                        Rule{
                            PARSER_RULE_ALTERNATIVE,
                            OR,
                            PARSER_RULE,
                        },
                        Rule{
                            PARSER_RULE,
                        },
                    },
                },
                {
                    PARSER_RULE,
                    {
                        Rule{
                            IDENTIFIER,
                        },
                        Rule{
                            STRING,
                        },
                        Rule{
                            ANY_PLACE_STRING,
                        },
                        Rule{
                            IDENTIFIER,
                            PARSER_RULE,
                        },
                        Rule{
                            STRING,
                            PARSER_RULE,
                        },
                        Rule{
                            ANY_PLACE_STRING,
                            PARSER_RULE,
                        },
                    },
                },
            },
        };

        return rules_grammar;
    }

    RulesReader::RulesReader(
        const std::map<std::string, SmallId> &default_rules, const isl::string_view input,
        const isl::string_view filename)
      : rulesConstructor{default_rules}
      , readerOptions{
            ReaderOption::WARNINGS_ENABLE,
            ReaderOption::SUGGESTIONS_ENABLE,
        }
    {
        using namespace std::chrono_literals;
        auto tokenizer = getRulesLexer().getTokenizer(input, filename);

        const auto id_to_str = ccl::lexer::lexerEnumToString<RulesLexerToken>;

        const auto gll_parser = GllParser(GOAL, getRulesGrammar(), id_to_str);
        auto [roots, algorithm] = gll_parser.parse(tokenizer);

        if (roots.size() != 1) {
            throw std::runtime_error{"Unable to construct unambiguous tree"};
        }

        auto root = isl::staticPointerCast<parser::ast::NonTerminal>(std::move(roots.front()));
        root->cast(getConversionTable());

        static_cast<ast::RulesReaderNode *>(root.get())->construct(rulesConstructor);
    }

    auto RulesReader::getParserBuilder() -> ParserBuilder &
    {
        return rulesConstructor;
    }
}// namespace ccl::parser::reader

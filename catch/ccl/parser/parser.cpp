#include <ccl/debug/debug_file.hpp>
#include <ccl/handler/cmd_handler.hpp>
#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/parser/parser.hpp>

using namespace ccl;

// NOLINTNEXTLINE
CCL_ENUM(// NOLINTNEXTLINE
    TestToken, size_t, EOI, BAD_TOKEN, ROOT, ID, NUM, MUL, ADD, NOT, ANGLE_OPENING, ANGLE_CLOSING,
    FACTOR, EXPR);

// NOLINTNEXTLINE
lex::LexicalAnalyzer LexicalAnalyzer(
    handler::Cmd::instance(),
    {
        { TestToken::ID, "[a-zA-Z_][a-zA-Z0-9_]*" },
        { TestToken::NUM, "[0-9]+" },
        { TestToken::MUL, "! [*]" },
        { TestToken::ADD, "! [+]" },
        { TestToken::NOT, "! [!]" },
        { TestToken::ANGLE_CLOSING, R"(! "]" )" },
        { TestToken::ANGLE_OPENING, R"(! "[" )" },
    });

struct FactorWithValue : parser::Factor<TestToken::FACTOR>
{
    FactorConstructor(FactorWithValue, TestToken::FACTOR);
};

struct ExpressionWithValue : parser::ValueExpression<TestToken::EXPR>
{
    ValueExpressionConstructor(ExpressionWithValue, TestToken::EXPR);
};

struct BinaryExpression : parser::BinaryExpression<TestToken::EXPR>
{
    BinaryExpressionConstructor(BinaryExpression, TestToken::EXPR);
};

struct Root : parser::ValueExpression<TestToken::ROOT>
{
    ValueExpressionConstructor(Root, TestToken::ROOT);
};

auto constructFactorFromID(parser::ParsingStack stack) -> parser::NodePtr
{
    return std::make_unique<FactorWithValue>(stack.pop());
}

auto constructExpressionFromFactor(parser::ParsingStack stack) -> parser::NodePtr
{
    return std::make_unique<ExpressionWithValue>(stack.pop());
}

auto constructBinaryExpression(parser::ParsingStack stack) -> parser::NodePtr
{
    return std::make_unique<BinaryExpression>(stack.pop(), stack.pop(), stack.pop());
}

auto constructRoot(parser::ParsingStack stack) -> parser::NodePtr
{
    return std::make_unique<Root>(stack.pop());
}

BOOST_AUTO_TEST_CASE(CclParser)
{
    auto tokenizer = LexicalAnalyzer.getTokenizer("10 + 10 * 2");

    auto rules = parser::ParsingRules(
        { CCL_PARSING_RULE(TestToken::FACTOR, constructFactorFromID, TestToken::NUM),
          CCL_PARSING_RULE(TestToken::EXPR, constructExpressionFromFactor, TestToken::FACTOR),
          ccl::parser::ParsingRule(
              TestToken::EXPR, "TestToken::EXPR", constructBinaryExpression,
              { TestToken::EXPR, TestToken::ADD, TestToken::EXPR }, { TestToken::MUL }),
          CCL_PARSING_RULE(
              TestToken::EXPR, constructBinaryExpression, TestToken::EXPR, TestToken::MUL,
              TestToken::EXPR),
          CCL_PARSING_RULE(TestToken::ROOT, constructRoot, TestToken::EXPR, TestToken::EOI) });

    auto parser = parser::Parser(rules, tokenizer);
    parser.parse();
}

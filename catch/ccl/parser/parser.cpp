#include <ccl/debug/debug_file.hpp>
#include <ccl/handler/cmd_handler.hpp>
#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/parser/parser.hpp>

using namespace ccl;

// NOLINTNEXTLINE
CCL_ENUM(// NOLINTNEXTLINE
    TestToken, size_t, EOI, BAD_TOKEN, ROOT, FUNC, ID, NUM, MUL, ADD, NOT, ANGLE_OPENING,
    ANGLE_CLOSING, FACTOR, EXPR);

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

struct UnaryExpression : parser::UnaryExpression<TestToken::FACTOR>
{
    UnaryExpressionConstructor(UnaryExpression, TestToken::FACTOR);
};

struct BinaryExpression : parser::BinaryExpression<TestToken::EXPR>
{
    BinaryExpressionConstructor(BinaryExpression, TestToken::EXPR);
};

struct Root : parser::ValueExpression<TestToken::ROOT>
{
    ValueExpressionConstructor(Root, TestToken::ROOT);
};

auto constructFactorFromID(parser::ParsingStack stack) -> UniquePtr<parser::Node>
{
    return makeUnique<parser::Node, FactorWithValue>(stack.pop());
}

auto constructExpressionFromFactor(parser::ParsingStack stack) -> UniquePtr<parser::Node>
{
    return makeUnique<parser::Node, ExpressionWithValue>(stack.pop());
}

auto constructUnaryExpression(parser::ParsingStack stack) -> UniquePtr<parser::Node>
{
    return makeUnique<parser::Node, UnaryExpression>(stack.pop(), stack.pop());
}

auto constructBinaryExpression(parser::ParsingStack stack) -> UniquePtr<parser::Node>
{
    return makeUnique<parser::Node, BinaryExpression>(stack.pop(), stack.pop(), stack.pop());
}

auto constructRoot(parser::ParsingStack stack) -> UniquePtr<parser::Node>
{
    return makeUnique<Root>(stack.pop());
}

auto constructIndexOperator(parser::ParsingStack stack) -> UniquePtr<parser::Node>
{
    auto angle_closing = stack.pop();
    auto expression = stack.pop();
    auto angle_opening = stack.pop();
    auto factor = stack.pop();

    return makeUnique<parser::Node, UnaryExpression>(std::move(factor), std::move(expression));
}

// NOLINTBEGIN
const ccl::UnorderedMap<ccl::parser::RuleId, size_t> PrecedenceTable(//
    { { TestToken::MUL, 30 }, { TestToken::ADD, 10 } });
// NOLINTEND

BOOST_AUTO_TEST_CASE(CclParser)
{
    auto rules = parser::ParsingRules(
        PrecedenceTable,
        { CCL_PARSING_RULE(TestToken::FACTOR, constructFactorFromID, TestToken::NUM),
          CCL_PARSING_RULE(TestToken::FACTOR, constructFactorFromID, TestToken::ID),
          CCL_PARSING_RULE(
              TestToken::FACTOR, constructIndexOperator, TestToken::FACTOR,
              TestToken::ANGLE_OPENING, TestToken::EXPR, TestToken::ANGLE_CLOSING),
          CCL_PARSING_RULE(TestToken::EXPR, constructExpressionFromFactor, TestToken::FACTOR),
          CCL_PARSING_RULE(
              TestToken::EXPR, constructBinaryExpression, TestToken::EXPR, TestToken::ADD,
              TestToken::EXPR),
          CCL_PARSING_RULE(
              TestToken::EXPR, constructBinaryExpression, TestToken::EXPR, TestToken::MUL,
              TestToken::EXPR),
          CCL_PARSING_RULE(TestToken::ROOT, constructRoot, TestToken::EXPR, TestToken::EOI),
          CCL_PARSING_RULE(TestToken::ROOT, constructRoot, TestToken::EOI) });


    auto tokenizer = LexicalAnalyzer.getTokenizer("");

    auto parser = parser::Parser(rules, tokenizer);
    parser.parse();
}

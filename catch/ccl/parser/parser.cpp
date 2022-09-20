#include <ccl/debug/debug_file.hpp>
#include <ccl/handler/cmd_handler.hpp>
#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/parser/parser.hpp>
#include <ccl/parser/parser2.hpp>

using namespace ccl;


CCL_ENUM(// NOLINTNEXTLINE
    TestToken, size_t, EOI, BAD_TOKEN, ID, NUM, MUL, ADD, NOT, ANGLE_OPENING, ANGLE_CLOSING, FACTOR,
    EXPR);

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

struct ValueFactor : public parser::ValueExpression<TestToken::FACTOR>
{
    ValueExpressionConstructor(ValueFactor, TestToken::FACTOR);
};

struct ValueExpression : public parser::ValueExpression<TestToken::EXPR>
{
    ValueExpressionConstructor(ValueExpression, TestToken::EXPR);
};

struct UnaryExpression : public parser::UnaryExpression<TestToken::EXPR>
{
    UnaryExpressionConstructor(UnaryExpression, TestToken::EXPR);
};

struct BinaryExpression : public parser::BinaryExpression<TestToken::EXPR>
{
    BinaryExpressionConstructor(BinaryExpression, TestToken::EXPR);
};

auto expressionFromNumConstruction(parser::StackGetter stack) -> parser::NodePtr
{
    return std::make_unique<ValueExpression>(stack.pop());
}

auto unaryExpressionConstruction(parser::StackGetter stack) -> parser::NodePtr
{
    return std::make_unique<UnaryExpression>(stack.pop(), stack.pop());
}

auto binaryExpressionConstruction(parser::StackGetter stack) -> parser::NodePtr
{
    return std::make_unique<BinaryExpression>(stack.pop(), stack.pop(), stack.pop());
}


auto factorFromNumConstruction(parser::StackGetter2 stack) -> parser::NodePtr
{
    return std::make_unique<ValueFactor>(stack.pop());
}

auto expressionFromFactorConstruction(parser::StackGetter2 stack) -> parser::NodePtr
{
    return std::make_unique<ValueExpression>(stack.pop());
}

auto unaryExpressionConstruction(parser::StackGetter2 stack) -> parser::NodePtr
{
    return std::make_unique<UnaryExpression>(stack.pop(), stack.pop());
}

auto indexOperatorConstruction(parser::StackGetter2 stack) -> parser::NodePtr
{
    auto angle_closing = stack.pop();
    auto expr = stack.pop();
    auto angle_opening = stack.pop();
    auto factor = stack.pop();

    return std::make_unique<UnaryExpression>(std::move(factor), std::move(expr));
}

auto binaryExpressionConstruction(parser::StackGetter2 stack) -> parser::NodePtr
{
    return std::make_unique<BinaryExpression>(stack.pop(), stack.pop(), stack.pop());
}


BOOST_AUTO_TEST_CASE(CclParser)
{
    auto tokenizer = LexicalAnalyzer.getTokenizer("a[10] + 2 * 4");

    std::unordered_map<size_t, size_t> precedence_table(//
        {
            { TestToken::NOT, 3 },
            { TestToken::MUL, 2 },
            { TestToken::ADD, 1 },
        });

    /*auto parser = parser::ParsingRules(//
        { { expressionFromNumConstruction, { TestToken::NUM } },
          { expressionFromNumConstruction, { TestToken::ID } },
          { unaryExpressionConstruction, { TestToken::NOT, TestToken::EXPR } },
          { binaryExpressionConstruction,
            { TestToken::EXPR, TestToken::ADD, TestToken::EXPR } },
          { binaryExpressionConstruction,
            { TestToken::EXPR, TestToken::MUL, TestToken::EXPR } } },
        precedence_table, TestToken::toString);
*/
    // parser.getParser(tokenizer).parse();

    auto parsing_rules = parser::ParsingRules2(//
        { { TestToken::FACTOR, factorFromNumConstruction, { TestToken::NUM } },
          { TestToken::FACTOR, factorFromNumConstruction, { TestToken::ID } },
          { TestToken::EXPR, expressionFromFactorConstruction, { TestToken::FACTOR } },
          { TestToken::EXPR, unaryExpressionConstruction, { TestToken::NOT, TestToken::EXPR } },
          { TestToken::EXPR,
            binaryExpressionConstruction,
            { TestToken::EXPR, TestToken::ADD, TestToken::EXPR } },
          { TestToken::EXPR,
            binaryExpressionConstruction,
            { TestToken::EXPR, TestToken::MUL, TestToken::EXPR } },
          { TestToken::FACTOR,
            indexOperatorConstruction,
            { TestToken::FACTOR, TestToken::ANGLE_OPENING, TestToken::EXPR,
              TestToken::ANGLE_CLOSING } } },
        precedence_table,
        TestToken::toString);

    auto parser = parser::Parser2(parsing_rules, tokenizer);
    parser.parser();
}

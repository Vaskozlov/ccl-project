#include <ccl/debug/debug_file.hpp>
#include <ccl/handler/cmd_handler.hpp>
#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/parser/parser.hpp>
#include <ccl/parser/parser2.hpp>

using namespace ccl;


CCL_ENUM(// NOLINTNEXTLINE
    TestIntEnum, size_t, EOI, BAD_TOKEN, ID, NUM, MUL, ADD, NOT, ANGLE_OPENING, ANGLE_CLOSING,
    FACTOR, EXPR);

// NOLINTNEXTLINE
lex::LexicalAnalyzer LexicalAnalyzer(
    handler::Cmd::instance(),
    {
        { TestIntEnum::ID, "[a-zA-Z_][a-zA-Z0-9_]*" },
        { TestIntEnum::NUM, "[0-9]+" },
        { TestIntEnum::MUL, "! [*]" },
        { TestIntEnum::ADD, "! [+]" },
        { TestIntEnum::NOT, "! [!]" },
        { TestIntEnum::ANGLE_CLOSING, R"(! "]" )" },
        { TestIntEnum::ANGLE_OPENING, R"(! "[" )" },
    });

struct ValueFactor : public parser::ValueExpression<TestIntEnum::FACTOR>
{
    ValueExpressionConstructor(ValueFactor, TestIntEnum::FACTOR);
};

struct ValueExpression : public parser::ValueExpression<TestIntEnum::EXPR>
{
    ValueExpressionConstructor(ValueExpression, TestIntEnum::EXPR);
};

struct UnaryExpression : public parser::UnaryExpression<TestIntEnum::EXPR>
{
    UnaryExpressionConstructor(UnaryExpression, TestIntEnum::EXPR);
};

struct BinaryExpression : public parser::BinaryExpression<TestIntEnum::EXPR>
{
    BinaryExpressionConstructor(BinaryExpression, TestIntEnum::EXPR);
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
    auto tokenizer = LexicalAnalyzer.getTokenizer("a[10] + 2");

    std::unordered_map<size_t, size_t> precedence_table(//
        {
            { TestIntEnum::NOT, 3 },
            { TestIntEnum::MUL, 2 },
            { TestIntEnum::ADD, 1 },
        });

    /*auto parser = parser::ParsingRules(//
        { { expressionFromNumConstruction, { TestIntEnum::NUM } },
          { expressionFromNumConstruction, { TestIntEnum::ID } },
          { unaryExpressionConstruction, { TestIntEnum::NOT, TestIntEnum::EXPR } },
          { binaryExpressionConstruction,
            { TestIntEnum::EXPR, TestIntEnum::ADD, TestIntEnum::EXPR } },
          { binaryExpressionConstruction,
            { TestIntEnum::EXPR, TestIntEnum::MUL, TestIntEnum::EXPR } } },
        precedence_table, TestIntEnum::toString);
*/
    // parser.getParser(tokenizer).parse();

    auto parsing_rules = parser::ParsingRules2(//
        { { factorFromNumConstruction, { TestIntEnum::NUM } },
          { factorFromNumConstruction, { TestIntEnum::ID } },
          { expressionFromFactorConstruction, { TestIntEnum::FACTOR } },
          { unaryExpressionConstruction, { TestIntEnum::NOT, TestIntEnum::EXPR } },
          { binaryExpressionConstruction,
            { TestIntEnum::EXPR, TestIntEnum::ADD, TestIntEnum::EXPR } },
          { binaryExpressionConstruction,
            { TestIntEnum::EXPR, TestIntEnum::MUL, TestIntEnum::EXPR } },
          { indexOperatorConstruction,
            { TestIntEnum::FACTOR, TestIntEnum::ANGLE_OPENING, TestIntEnum::EXPR,
              TestIntEnum::ANGLE_CLOSING } } },
        precedence_table,
        TestIntEnum::toString);

    auto parser = parser::Parser2(parsing_rules, tokenizer);
    parser.parser();
}

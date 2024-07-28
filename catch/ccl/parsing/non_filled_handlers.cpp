#include "basic_math_grammar.hpp"

/*
 * Try with grammar that we know already works
 */
TEST_CASE("NonFilledHandlersMathGrammarComputation", "[NonFilledHandlers]") {

    constexpr auto start_symbol = ccl::debug::GrammarSymbol::GOAL;
    constexpr auto end_symbol = ccl::debug::GrammarSymbol::EOI;
    auto math_grammar = ccl::parser::GrammarRulesStorage(ccl::debug::GrammarForFirstFollowSetTest);

    const auto set = math_grammar.getNotFilledHandlers(start_symbol, end_symbol);

    REQUIRE(set.empty());
}


/*
 * Try with bad grammar
 */
TEST_CASE("NonFilledHandlersBadGrammarComputation", "[NonFilledHandlers]") {

    using GrammarSymbol = ccl::debug::GrammarSymbol;
    using Rule = ccl::parser::Rule;

    constexpr auto start_symbol = ccl::debug::GrammarSymbol::GOAL;
    constexpr auto end_symbol = ccl::debug::GrammarSymbol::EOI;

    auto DUMMY_TOKEN_ONE = 12345ULL;
    auto DUMMY_TOKEN_TWO = 67890ULL;


    auto grammar_storage = ccl::parser::GrammarRulesStorage{
        GrammarSymbol::EPSILON,
        {
            {
                GrammarSymbol::GOAL,
                {
                    Rule{
                        {
                            GrammarSymbol::EXPR,
                        },
                    },
                },
            },
            {
                GrammarSymbol::EXPR,
                {
                    Rule{
                        {
                            GrammarSymbol::TERM,
                            GrammarSymbol::EXPR_C,
                        },
                    },
                },
            },
            {
                GrammarSymbol::EXPR_C,
                {
                    Rule{
                        {
                            GrammarSymbol::ADD,
                            GrammarSymbol::TERM,
                            GrammarSymbol::EXPR_C,
                        },
                    },
                    Rule{
                        {
                            GrammarSymbol::SUB,
                            GrammarSymbol::TERM,
                            GrammarSymbol::EXPR_C,
                        },
                    },
                    Rule{
                        {GrammarSymbol::EPSILON},
                    },
                },
            },
            {
                GrammarSymbol::TERM,
                {
                    Rule{
                        {
                            GrammarSymbol::FACTOR,
                            GrammarSymbol::TERM_C,
                        },
                    },
                },
            },
            {
                GrammarSymbol::TERM_C,
                {
                    Rule{
                        {
                            GrammarSymbol::MUL,
                            GrammarSymbol::FACTOR,
                            GrammarSymbol::TERM_C,
                        },
                    },
                    Rule{
                        {
                            GrammarSymbol::DIV,
                            GrammarSymbol::FACTOR,
                            GrammarSymbol::TERM_C,
                        },
                    },
                    Rule{
                        {
                            GrammarSymbol::EPSILON,
                        },
                    },
                },
            },
            {
                GrammarSymbol::FACTOR,
                {
                    Rule{
                        {
                            GrammarSymbol::ANGLE_OPEN,
                            GrammarSymbol::EXPR,
                            GrammarSymbol::ANGLE_CLOSE,
                        },
                    },
                    Rule{
                        {
                            GrammarSymbol::NAME,
                        },
                    },
                    Rule{
                        {
                            GrammarSymbol::NUMBER,
                        },
                    },
                },
            },
            {
                /*
                 * dummy value to test empty handler
                 * will be removed by fix empty? ans : no
                 */
                DUMMY_TOKEN_ONE,
                {},
            },
            {
                /*
                 * another dummy value, has non empty rule vector
                 * but no terminal values following
                 * (that's why we follow set)
                 */
                DUMMY_TOKEN_TWO,
                {
                    Rule{{DUMMY_TOKEN_ONE}},
                },
            },
        }};


    const auto set = grammar_storage
        .getNotFilledHandlers(start_symbol, end_symbol);

    REQUIRE(set == std::set<ccl::parser::Symbol>{
        DUMMY_TOKEN_ONE,
        DUMMY_TOKEN_TWO,
    });
}
#ifndef CCL_PROJECT_BASIC_MATH_GRAMMAR_HPP
#define CCL_PROJECT_BASIC_MATH_GRAMMAR_HPP

#include "basic_math.hpp"
#include <ccl/parser/grammar_rules_storage.hpp>

namespace ccl::debug
{
    using GrammarSymbol = math::MathLexerToken;

    const inline parser::GrammarStorage GrammarForFirstFollowSetTest{
        false,
        GrammarSymbol::EPSILON,
        {
            {
                GrammarSymbol::GOAL,
                {
                    parser::Rule{
                        {
                            GrammarSymbol::EXPR,
                        },
                    },
                },
            },
            {
                GrammarSymbol::EXPR,
                {
                    parser::Rule{
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
                    parser::Rule{
                        {
                            GrammarSymbol::ADD,
                            GrammarSymbol::TERM,
                            GrammarSymbol::EXPR_C,
                        },
                    },
                    parser::Rule{
                        {
                            GrammarSymbol::SUB,
                            GrammarSymbol::TERM,
                            GrammarSymbol::EXPR_C,
                        },
                    },
                    parser::Rule{
                        {
                            GrammarSymbol::EPSILON,
                        },
                    },
                },
            },
            {
                GrammarSymbol::TERM,
                {
                    parser::Rule{
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
                    parser::Rule{
                        {
                            GrammarSymbol::MUL,
                            GrammarSymbol::FACTOR,
                            GrammarSymbol::TERM_C,
                        },
                    },
                    parser::Rule{
                        {
                            GrammarSymbol::DIV,
                            GrammarSymbol::FACTOR,
                            GrammarSymbol::TERM_C,
                        },
                    },
                    parser::Rule{
                        {
                            GrammarSymbol::EPSILON,
                        },
                    },
                },
            },
            {
                GrammarSymbol::FACTOR,
                {
                    parser::Rule{
                        {
                            GrammarSymbol::ANGLE_OPEN,
                            GrammarSymbol::EXPR,
                            GrammarSymbol::ANGLE_CLOSE,
                        },
                    },
                    parser::Rule{
                        {
                            GrammarSymbol::NAME,
                        },
                    },
                    parser::Rule{
                        {
                            GrammarSymbol::NUMBER,
                        },
                    },
                },
            },
        },
    };
}// namespace ccl::debug

template<>
struct fmt::formatter<ccl::debug::GrammarSymbol> : formatter<std::string_view>
{
    constexpr auto format(const ccl::debug::GrammarSymbol &symbol, format_context &ctx) const
        -> format_context::iterator
    {
        return fmt::format_to(ctx.out(), "{}", ccl::lexer::lexerEnumToString(symbol));
    }
};

#endif /* CCL_PROJECT_BASIC_MATH_GRAMMAR_HPP */

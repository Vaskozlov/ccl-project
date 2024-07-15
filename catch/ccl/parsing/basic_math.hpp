#pragma once

#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <isl/flatmap.hpp>

namespace math
{
    enum MathLexerToken : ccl::Id;
}

template<>
CCL_DECL auto ccl::lexer::lexerEnumToString<math::MathLexerToken>(math::MathLexerToken value)
    -> isl::string_view;

namespace math
{
    enum MathLexerToken : ccl::Id
    {
        NONE = 0ULL,
        EOI = 0ULL,
        BAD_TOKEN = 1ULL,
        CUT = 2ULL,
        ANGLE_OPEN = 3ULL,
        ANGLE_CLOSE = 4ULL,
        ADD = 5ULL,
        SUB = 6ULL,
        MUL = 7ULL,
        DIV = 8ULL,
        NUMBER = 9ULL,
        NAME = 10ULL,
        GOAL = 11ULL,
        EPSILON = 12ULL,
        EXPR = 13ULL,
        EXPR_C = 14ULL,
        TERM = 15ULL,
        TERM_C = 16ULL,
        VALUE = 17ULL,
        FACTOR = 18ULL,
    };

    // NOLINTNEXTLINE
    inline auto MathLexer = ccl::lexer::LexicalAnalyzer{
        ccl::handler::Cmd::instance(),
        {
            {MathLexerToken::ANGLE_OPEN, R"( ![(] )"},
            {MathLexerToken::ANGLE_CLOSE, R"( ![)] )"},
            {MathLexerToken::ADD, R"( ![+] )"},
            {MathLexerToken::SUB, R"( ![\-] )"},
            {MathLexerToken::MUL, R"( ![*] )"},
            {MathLexerToken::DIV, R"( ![/] )"},
            {MathLexerToken::NUMBER, R"( [0-9]+ )"},
            {MathLexerToken::NAME, R"( [a-zA-Z_][a-zA-Z0-9_]* )"},
            {MathLexerToken::GOAL, R"( [0] )"},
            {MathLexerToken::EPSILON, R"( [0] )"},
            {MathLexerToken::EXPR, R"( [0] )"},
            {MathLexerToken::EXPR_C, R"( [0] )"},
            {MathLexerToken::TERM, R"( [0] )"},
            {MathLexerToken::TERM_C, R"( [0] )"},
            {MathLexerToken::VALUE, R"( [0] )"},
            {MathLexerToken::FACTOR, R"( [0] )"},
        }};

    inline constexpr isl::StaticFlatmap<ccl::Id, isl::string_view, 19> ToStringMathLexerToken{
        {MathLexerToken::EOI, "EOI"},
        {MathLexerToken::BAD_TOKEN, "BAD_TOKEN"},
        {MathLexerToken::CUT, "CUT"},
        {MathLexerToken::ANGLE_OPEN, "ANGLE_OPEN"},
        {MathLexerToken::ANGLE_CLOSE, "ANGLE_CLOSE"},
        {MathLexerToken::ADD, "ADD"},
        {MathLexerToken::SUB, "SUB"},
        {MathLexerToken::MUL, "MUL"},
        {MathLexerToken::DIV, "DIV"},
        {MathLexerToken::NUMBER, "NUMBER"},
        {MathLexerToken::NAME, "NAME"},
        {MathLexerToken::GOAL, "GOAL"},
        {MathLexerToken::EPSILON, "EPSILON"},
        {MathLexerToken::EXPR, "EXPR"},
        {MathLexerToken::EXPR_C, "EXPR_C"},
        {MathLexerToken::TERM, "TERM"},
        {MathLexerToken::TERM_C, "TERM_C"},
        {MathLexerToken::VALUE, "VALUE"},
        {MathLexerToken::FACTOR, "FACTOR"},
    };
}// namespace math

namespace ccl::lexer
{
    template<>
    CCL_DECL auto
        lexerEnumToString<math::MathLexerToken>(math::MathLexerToken value) -> isl::string_view
    {
        return math::ToStringMathLexerToken[value];
    }
}// namespace ccl::lexer

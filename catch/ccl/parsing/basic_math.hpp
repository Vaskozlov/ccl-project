#pragma once

#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <isl/static_flat_map.hpp>

namespace math
{
    enum MathLexerToken : ccl::SmallId;
}

template<>
CCL_DECL auto ccl::lexer::lexerEnumToString<math::MathLexerToken>(SmallId value) -> std::string;

namespace math
{
    enum MathLexerToken : ccl::SmallId
    {
        EOI = 0U,
        BAD_TOKEN = 1U,
        CUT = 2U,
        ANGLE_OPEN = 3U,
        ANGLE_CLOSE = 4U,
        ADD = 5U,
        SUB = 6U,
        MUL = 7U,
        DIV = 8U,
        NUMBER = 9U,
        NAME = 10U,
        GOAL = 11U,
        EPSILON = 12U,
        EXPR = 13U,
        EXPR_C = 14U,
        TERM = 15U,
        TERM_C = 16U,
        VALUE = 17U,
        FACTOR = 18U,
    };

    // NOLINTNEXTLINE
    inline auto const MathLexer = ccl::lexer::LexicalAnalyzer{
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

    inline constexpr isl::StaticFlatmap<ccl::SmallId, isl::string_view, 19> ToStringMathLexerToken{
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
    CCL_DECL auto lexerEnumToString<math::MathLexerToken>(SmallId value) -> std::string
    {
        return static_cast<std::string>(math::ToStringMathLexerToken[value]);
    }
}// namespace ccl::lexer

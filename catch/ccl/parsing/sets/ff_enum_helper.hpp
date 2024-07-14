#pragma once

#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <isl/flatmap.hpp>

namespace ff_test
{
    enum helperToken : ccl::Id;
}

template<>
CCL_DECL auto ccl::lexer::lexerEnumToString<ff_test::helperToken>(ff_test::helperToken value)
    -> isl::string_view;

namespace ff_test
{
    enum helperToken : ccl::Id
    {
        NONE = 0,
        EOI = 0,
        BAD_TOKEN = 1,
        CUT = 2,
        ANGLE_OPEN = 3,
        ANGLE_CLOSE = 4,
        ADD = 5,
        SUB = 6,
        MUL = 7,
        DIV = 8,
        NUMBER = 9,
        NAME = 10,
        GOAL = 11,
        EPSILON = 12,
        EXPR = 13,
        EXPR_C = 14,
        TERM = 15,
        TERM_C = 16,
        FACTOR = 17,
    };

    // NOLINTNEXTLINE
    inline auto helper = ccl::lexer::LexicalAnalyzer{
        ccl::handler::Cmd::instance(),
        {
            {helperToken::ANGLE_OPEN, R"( ![(] )"},
            {helperToken::ANGLE_CLOSE, R"( ![)] )"},
            {helperToken::ADD, R"( ![+] )"},
            {helperToken::SUB, R"( ![\-] )"},
            {helperToken::MUL, R"( ![*] )"},
            {helperToken::DIV, R"( ![/] )"},
            {helperToken::NUMBER, R"( [0-9]+ )"},
            {helperToken::NAME, R"( [a-zA-Z_][a-zA-Z0-9_]* )"},
            {helperToken::GOAL, R"( [0] )"},
            {helperToken::EPSILON, R"( [0] )"},
            {helperToken::EXPR, R"( [0] )"},
            {helperToken::EXPR_C, R"( [0] )"},
            {helperToken::TERM, R"( [0] )"},
            {helperToken::TERM_C, R"( [0] )"},
            {helperToken::FACTOR, R"( [0] )"},
        }};

    inline constexpr isl::StaticFlatmap<ccl::Id, isl::string_view, 18> ToStringhelperToken{
        {helperToken::EOI, "EOI"},
        {helperToken::BAD_TOKEN, "BAD_TOKEN"},
        {helperToken::CUT, "CUT"},
        {helperToken::ANGLE_OPEN, "ANGLE_OPEN"},
        {helperToken::ANGLE_CLOSE, "ANGLE_CLOSE"},
        {helperToken::ADD, "ADD"},
        {helperToken::SUB, "SUB"},
        {helperToken::MUL, "MUL"},
        {helperToken::DIV, "DIV"},
        {helperToken::NUMBER, "NUMBER"},
        {helperToken::NAME, "NAME"},
        {helperToken::GOAL, "GOAL"},
        {helperToken::EPSILON, "EPSILON"},
        {helperToken::EXPR, "EXPR"},
        {helperToken::EXPR_C, "EXPR_C"},
        {helperToken::TERM, "TERM"},
        {helperToken::TERM_C, "TERM_C"},
        {helperToken::FACTOR, "FACTOR"},
    };
}// namespace ff_test

namespace ccl::lexer
{
    template<>
    CCL_DECL auto
        lexerEnumToString<ff_test::helperToken>(ff_test::helperToken value) -> isl::string_view
    {
        return ff_test::ToStringhelperToken[value];
    }
}// namespace ccl::lexer

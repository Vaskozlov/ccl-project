#pragma once

#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <isl/flatmap.hpp>

namespace tparser
{
    enum class TestParserToken : ccl::SmallId;
}

template<>
CCL_DECL auto ccl::lexer::lexerEnumToString<tparser::TestParserToken>(
    tparser::TestParserToken value) -> isl::string_view;

namespace tparser
{
    enum class TestParserToken : ccl::SmallId
    {
        NONE = 0,
        EOI = 0,
        BAD_TOKEN = 1,
        CUT = 2,
        PRODUCT = 3,
        ADDITION = 4,
        EXPR = 5,
        NUMBER = 6,
        PARENTHESIZED_EXPR = 7,
    };

    // NOLINTNEXTLINE
    inline auto TestParser = ccl::lexer::LexicalAnalyzer{
        ccl::handler::Cmd::instance(),
        {
            {TestParserToken::PRODUCT, R"( ('NUMBER'  [*] 'NUMBER') | 'NUMBER' )"},
            {TestParserToken::ADDITION, R"( ('PRODUCT' [+] 'ADDITION') | 'PRODUCT' )"},
            {TestParserToken::EXPR, R"( 'ADDITION' )"},
            {TestParserToken::NUMBER, R"( [0-9]+ | 'PARENTHESIZED_EXPR' )"},
            {TestParserToken::PARENTHESIZED_EXPR, R"( ([(] 'EXPR' [)]) )"},
        }};

    inline constexpr isl::StaticFlatmap<TestParserToken, isl::string_view, 8>
        ToStringTestParserToken{
            {TestParserToken::EOI, "EOI"},
            {TestParserToken::BAD_TOKEN, "BAD_TOKEN"},
            {TestParserToken::CUT, "CUT"},
            {TestParserToken::PRODUCT, "PRODUCT"},
            {TestParserToken::ADDITION, "ADDITION"},
            {TestParserToken::EXPR, "EXPR"},
            {TestParserToken::NUMBER, "NUMBER"},
            {TestParserToken::PARENTHESIZED_EXPR, "PARENTHESIZED_EXPR"},
        };
}// namespace tparser

namespace ccl::lexer
{
    template<>
    CCL_DECL auto lexerEnumToString<tparser::TestParserToken>(tparser::TestParserToken value)
        -> isl::string_view
    {
        return tparser::ToStringTestParserToken[value];
    }
}// namespace ccl::lexer

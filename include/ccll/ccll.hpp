#pragma once

#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <isl/flatmap.hpp>

namespace ccll
{
    enum CcllAnalyzerToken : ccl::Id;
}

template<>
CCL_DECL auto ccl::lexer::lexerEnumToString<ccll::CcllAnalyzerToken>(ccll::CcllAnalyzerToken value)
    -> isl::string_view;

namespace ccll
{
    enum CcllAnalyzerToken : ccl::Id
    {
        EXTRA = 12884901888ULL,
        FAILURE = 8589934592ULL,
        NONE = 0ULL,
        SUCCESS = 4294967296ULL,
        EOI = 0ULL,
        BAD_TOKEN = 1ULL,
        CUT = 2ULL,
        GROUP_DECLARATION = 4294967296ULL,
        RULE = 4294967297ULL,
        DIRECTIVE = 4294967298ULL,
        BAD_GROUP_DECLARATION_EMPTY_NAME = 8589934592ULL,
        BAD_GROUP_DECLARATION_BRACKET_AND_NAME = 8589934593ULL,
        BAD_GROUP_NO_OPEN_BRACKET = 8589934594ULL,
        BAD_GROUP_DECLARATION_ONLY_BRACKET = 8589934595ULL,
        BAD_RULE_DECLARATION = 8589934596ULL,
        BAD_DIRECTIVE_DECLARATION = 8589934597ULL,
        BAD_RULE_OR_DIRECTIVE_DECLARATION = 8589934598ULL,
        COMMENT = 12884901888ULL,
    };

    // NOLINTNEXTLINE
    inline auto CcllAnalyzer = ccl::lexer::LexicalAnalyzer{
        ccl::handler::Cmd::instance(),
        {
            {CcllAnalyzerToken::GROUP_DECLARATION, R"( [\[]p ( [a-zA-Z_][a-zA-Z_0-9]* )p [\]]p )"},
            {CcllAnalyzerToken::RULE,
             R"( ( [a-zA-Z_][a-zA-Z_0-9]* )p [ \t]* [:] [ \t]* ( [\n]+^ )p )"},
            {CcllAnalyzerToken::DIRECTIVE,
             R"( ( [a-zA-Z_][a-zA-Z_0-9]* )p [ \t]* [=] [ \t]* ("\"" (["]^ | "\\\"" )* "\"")p )"},
            {CcllAnalyzerToken::BAD_GROUP_DECLARATION_EMPTY_NAME, R"( [\[] [\]] [\n]*^ )"},
            {CcllAnalyzerToken::BAD_GROUP_DECLARATION_BRACKET_AND_NAME,
             R"( [\[] [a-zA-Z_][a-zA-Z_0-9]*[\n]*^ )"},
            {CcllAnalyzerToken::BAD_GROUP_NO_OPEN_BRACKET,
             R"( [a-zA-Z_][a-zA-Z_0-9]* [\]] [\n]*^ )"},
            {CcllAnalyzerToken::BAD_GROUP_DECLARATION_ONLY_BRACKET, R"( [\[] [\n]*^ )"},
            {CcllAnalyzerToken::BAD_RULE_DECLARATION,
             R"( ( [a-zA-Z_][a-zA-Z_0-9]* )p [ \t]* [:] [\n]*^ )"},
            {CcllAnalyzerToken::BAD_DIRECTIVE_DECLARATION,
             R"( ( [a-zA-Z_][a-zA-Z_0-9]* )p [ \t]* [=] [\n]*^ )"},
            {CcllAnalyzerToken::BAD_RULE_OR_DIRECTIVE_DECLARATION,
             R"( [a-zA-Z_][a-zA-Z_0-9]* [\n]*^ )"},
            {CcllAnalyzerToken::COMMENT, R"( ! [#] [\n]*^ )"},
        }};

    inline constexpr isl::StaticFlatmap<ccl::Id, isl::string_view, 14> ToStringCcllAnalyzerToken{
        {CcllAnalyzerToken::EOI, "EOI"},
        {CcllAnalyzerToken::BAD_TOKEN, "BAD_TOKEN"},
        {CcllAnalyzerToken::CUT, "CUT"},
        {CcllAnalyzerToken::GROUP_DECLARATION, "GROUP_DECLARATION"},
        {CcllAnalyzerToken::RULE, "RULE"},
        {CcllAnalyzerToken::DIRECTIVE, "DIRECTIVE"},
        {CcllAnalyzerToken::BAD_GROUP_DECLARATION_EMPTY_NAME, "BAD_GROUP_DECLARATION_EMPTY_NAME"},
        {CcllAnalyzerToken::BAD_GROUP_DECLARATION_BRACKET_AND_NAME,
         "BAD_GROUP_DECLARATION_BRACKET_AND_NAME"},
        {CcllAnalyzerToken::BAD_GROUP_NO_OPEN_BRACKET, "BAD_GROUP_NO_OPEN_BRACKET"},
        {CcllAnalyzerToken::BAD_GROUP_DECLARATION_ONLY_BRACKET,
         "BAD_GROUP_DECLARATION_ONLY_BRACKET"},
        {CcllAnalyzerToken::BAD_RULE_DECLARATION, "BAD_RULE_DECLARATION"},
        {CcllAnalyzerToken::BAD_DIRECTIVE_DECLARATION, "BAD_DIRECTIVE_DECLARATION"},
        {CcllAnalyzerToken::BAD_RULE_OR_DIRECTIVE_DECLARATION, "BAD_RULE_OR_DIRECTIVE_DECLARATION"},
        {CcllAnalyzerToken::COMMENT, "COMMENT"},
    };
}// namespace ccll

namespace ccl::lexer
{
    template<>
    CCL_DECL auto lexerEnumToString<ccll::CcllAnalyzerToken>(ccll::CcllAnalyzerToken value)
        -> isl::string_view
    {
        return ccll::ToStringCcllAnalyzerToken[value];
    }
}// namespace ccl::lexer

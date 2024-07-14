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
        EXTRA = 12884901888,
        FAILURE = 8589934592,
        NONE = 0,
        SUCCESS = 4294967296,
        EOI = 0,
        BAD_TOKEN = 1,
        CUT = 2,
        GROUP_DECLARATION = 4294967296,
        RULE = 4294967297,
        DIRECTIVE = 4294967298,
        BAD_GROUP_DECLARATION_EMPTY_NAME = 8589934592,
        BAD_GROUP_DECLARATION_BRACKET_AND_NAME = 8589934593,
        BAD_GROUP_NO_OPEN_BRACKET = 8589934594,
        BAD_GROUP_DECLARATION_ONLY_BRACKET = 8589934595,
        BAD_RULE_DECLARATION = 8589934596,
        BAD_DIRECTIVE_DECLARATION = 8589934597,
        BAD_RULE_OR_DIRECTIVE_DECLARATION = 8589934598,
        COMMENT = 12884901888,
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

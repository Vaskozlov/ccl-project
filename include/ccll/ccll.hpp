#pragma once

#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <isl/static_flat_map.hpp>

namespace ccll
{
    enum CcllAnalyzerToken : ccl::u32;
}

template<>
CCL_DECL auto ccl::lexer::lexerEnumToString<ccll::CcllAnalyzerToken>(ccll::CcllAnalyzerToken value)
    -> isl::string_view;

namespace ccll
{
    enum CcllAnalyzerToken : ccl::u32
    {
        EOI = 0ULL,
        BAD_TOKEN = 1ULL,
        CUT = 2ULL,
        GROUP_DECLARATION = 3ULL,
        RULE = 4ULL,
        DIRECTIVE = 5ULL,
        BAD_GROUP_DECLARATION_EMPTY_NAME = 6ULL,
        BAD_GROUP_DECLARATION_BRACKET_AND_NAME = 7ULL,
        BAD_GROUP_NO_OPEN_BRACKET = 8ULL,
        BAD_GROUP_DECLARATION_ONLY_BRACKET = 9ULL,
        BAD_RULE_DECLARATION = 10ULL,
        BAD_DIRECTIVE_DECLARATION = 11ULL,
        BAD_RULE_OR_DIRECTIVE_DECLARATION = 12ULL,
        COMMENT = 13ULL,
    };

    // NOLINTNEXTLINE
    inline const auto CcllAnalyzer = ccl::lexer::LexicalAnalyzer{
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

    inline constexpr isl::StaticFlatmap<ccl::SmallId, isl::string_view, 14>
        ToStringCcllAnalyzerToken{
            {CcllAnalyzerToken::EOI, "EOI"},
            {CcllAnalyzerToken::BAD_TOKEN, "BAD_TOKEN"},
            {CcllAnalyzerToken::CUT, "CUT"},
            {CcllAnalyzerToken::GROUP_DECLARATION, "GROUP_DECLARATION"},
            {CcllAnalyzerToken::RULE, "RULE"},
            {CcllAnalyzerToken::DIRECTIVE, "DIRECTIVE"},
            {CcllAnalyzerToken::BAD_GROUP_DECLARATION_EMPTY_NAME,
             "BAD_GROUP_DECLARATION_EMPTY_NAME"},
            {CcllAnalyzerToken::BAD_GROUP_DECLARATION_BRACKET_AND_NAME,
             "BAD_GROUP_DECLARATION_BRACKET_AND_NAME"},
            {CcllAnalyzerToken::BAD_GROUP_NO_OPEN_BRACKET, "BAD_GROUP_NO_OPEN_BRACKET"},
            {CcllAnalyzerToken::BAD_GROUP_DECLARATION_ONLY_BRACKET,
             "BAD_GROUP_DECLARATION_ONLY_BRACKET"},
            {CcllAnalyzerToken::BAD_RULE_DECLARATION, "BAD_RULE_DECLARATION"},
            {CcllAnalyzerToken::BAD_DIRECTIVE_DECLARATION, "BAD_DIRECTIVE_DECLARATION"},
            {CcllAnalyzerToken::BAD_RULE_OR_DIRECTIVE_DECLARATION,
             "BAD_RULE_OR_DIRECTIVE_DECLARATION"},
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

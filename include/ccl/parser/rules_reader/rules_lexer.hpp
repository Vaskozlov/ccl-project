#pragma once

#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <isl/flatmap.hpp>

namespace ccl::parser::reader
{
    enum RulesLexerToken : ccl::Id;
}

template<>
CCL_DECL auto ccl::lexer::lexerEnumToString<ccl::parser::reader::RulesLexerToken>(
    ccl::parser::reader::RulesLexerToken value) -> isl::string_view;

namespace ccl::parser::reader
{
    enum RulesLexerToken : ccl::Id
    {
        NONE = 0ULL,
        PARSER_TOKENS = 8589934592ULL,
        SUCCESS = 4294967296ULL,
        EOI = 0ULL,
        BAD_TOKEN = 1ULL,
        CUT = 2ULL,
        LEXER_START = 4294967296ULL,
        PARSER_START = 4294967297ULL,
        OR = 4294967298ULL,
        STAR = 4294967299ULL,
        PLUS = 4294967300ULL,
        EXCLAMATION_POINT = 4294967301ULL,
        CURLY_OPEN = 4294967302ULL,
        CURLY_CLOSE = 4294967303ULL,
        COMMA = 4294967304ULL,
        NUMBER = 4294967305ULL,
        RULE_IDENTIFIER = 4294967306ULL,
        IDENTIFIER = 4294967307ULL,
        RULE_REFERENCE = 4294967308ULL,
        STRING = 4294967309ULL,
        UNION = 4294967310ULL,
        PREFIX_POSTFIX_OPERATOR = 4294967311ULL,
        HIDE_OPERATOR = 4294967312ULL,
        NOT_OPERATOR = 4294967313ULL,
        COLUMN = 4294967314ULL,
        GOAL = 8589934592ULL,
        EPSILON = 8589934593ULL,
        LEXER_BLOCK = 8589934594ULL,
        LEXER_BLOCK_DEFINITION = 8589934595ULL,
        PARSER_BLOCK = 8589934596ULL,
        PARSER_BLOCK_DEFINITION = 8589934597ULL,
        ANY_BLOCK = 8589934598ULL,
        LEXER_RULE = 8589934599ULL,
        PARSER_RULE = 8589934600ULL,
        PARSER_RULE_BODY = 8589934601ULL,
        PARSER_RULE_DECL = 8589934602ULL,
        PARSER_RULE_ALTERNATIVE = 8589934603ULL,
        LEXER_RULE_BODY = 8589934604ULL,
        LEXER_RULE_BLOCK = 8589934605ULL,
        LEXER_RULE_DECL = 8589934606ULL,
        LEXER_RULE_OPTIONS = 8589934607ULL,
        LEXER_RULE_ALTERNATIVE = 8589934608ULL,
    };

    // NOLINTNEXTLINE
    inline auto RulesLexer = ccl::lexer::LexicalAnalyzer{
        ccl::handler::Cmd::instance(),
        {
            {RulesLexerToken::LEXER_START, R"( "%LEXER%" )"},
            {RulesLexerToken::PARSER_START, R"( "%PARSER%" )"},
            {RulesLexerToken::OR, R"( ![|] )"},
            {RulesLexerToken::STAR, R"( ! [*] )"},
            {RulesLexerToken::PLUS, R"( ! [+] )"},
            {RulesLexerToken::EXCLAMATION_POINT, R"( ! [!] )"},
            {RulesLexerToken::CURLY_OPEN, R"( ! [{] )"},
            {RulesLexerToken::CURLY_CLOSE, R"( ! [}] )"},
            {RulesLexerToken::COMMA, R"( ! [,] )"},
            {RulesLexerToken::NUMBER, R"( [0-9]+ )"},
            {RulesLexerToken::RULE_IDENTIFIER, R"( 'IDENTIFIER'p [ \t]* [:] )"},
            {RulesLexerToken::IDENTIFIER, R"( [a-zA-Z_][a-zA-Z0-9_]* )"},
            {RulesLexerToken::RULE_REFERENCE, R"( ! [<] [>]^* [>] )"},
            {RulesLexerToken::STRING, R"( ! ["] (["]^ | "\\\"")* ["] )"},
            {RulesLexerToken::UNION, R"( ! [\[] ([\]]^ | "\\]")* [\]] )"},
            {RulesLexerToken::PREFIX_POSTFIX_OPERATOR, R"( ! [p] )"},
            {RulesLexerToken::HIDE_OPERATOR, R"( ! [h] )"},
            {RulesLexerToken::NOT_OPERATOR, R"( ! [^] )"},
            {RulesLexerToken::COLUMN, R"( ! [:] )"},
            {RulesLexerToken::GOAL, R"( [#] )"},
            {RulesLexerToken::EPSILON, R"( [#] )"},
            {RulesLexerToken::LEXER_BLOCK, R"( [#] )"},
            {RulesLexerToken::LEXER_BLOCK_DEFINITION, R"( [#] )"},
            {RulesLexerToken::PARSER_BLOCK, R"( [#] )"},
            {RulesLexerToken::PARSER_BLOCK_DEFINITION, R"( [#] )"},
            {RulesLexerToken::ANY_BLOCK, R"( [#] )"},
            {RulesLexerToken::LEXER_RULE, R"( [#] )"},
            {RulesLexerToken::PARSER_RULE, R"( [#] )"},
            {RulesLexerToken::PARSER_RULE_BODY, R"( [#] )"},
            {RulesLexerToken::PARSER_RULE_DECL, R"( [#] )"},
            {RulesLexerToken::PARSER_RULE_ALTERNATIVE, R"( [#] )"},
            {RulesLexerToken::LEXER_RULE_BODY, R"( [#] )"},
            {RulesLexerToken::LEXER_RULE_BLOCK, R"( [#] )"},
            {RulesLexerToken::LEXER_RULE_DECL, R"( [#] )"},
            {RulesLexerToken::LEXER_RULE_OPTIONS, R"( [#] )"},
            {RulesLexerToken::LEXER_RULE_ALTERNATIVE, R"( [#] )"},
        }};

    inline constexpr isl::StaticFlatmap<ccl::Id, isl::string_view, 39> ToStringRulesLexerToken{
        {RulesLexerToken::EOI, "EOI"},
        {RulesLexerToken::BAD_TOKEN, "BAD_TOKEN"},
        {RulesLexerToken::CUT, "CUT"},
        {RulesLexerToken::LEXER_START, "LEXER_START"},
        {RulesLexerToken::PARSER_START, "PARSER_START"},
        {RulesLexerToken::OR, "OR"},
        {RulesLexerToken::STAR, "STAR"},
        {RulesLexerToken::PLUS, "PLUS"},
        {RulesLexerToken::EXCLAMATION_POINT, "EXCLAMATION_POINT"},
        {RulesLexerToken::CURLY_OPEN, "CURLY_OPEN"},
        {RulesLexerToken::CURLY_CLOSE, "CURLY_CLOSE"},
        {RulesLexerToken::COMMA, "COMMA"},
        {RulesLexerToken::NUMBER, "NUMBER"},
        {RulesLexerToken::RULE_IDENTIFIER, "RULE_IDENTIFIER"},
        {RulesLexerToken::IDENTIFIER, "IDENTIFIER"},
        {RulesLexerToken::RULE_REFERENCE, "RULE_REFERENCE"},
        {RulesLexerToken::STRING, "STRING"},
        {RulesLexerToken::UNION, "UNION"},
        {RulesLexerToken::PREFIX_POSTFIX_OPERATOR, "PREFIX_POSTFIX_OPERATOR"},
        {RulesLexerToken::HIDE_OPERATOR, "HIDE_OPERATOR"},
        {RulesLexerToken::NOT_OPERATOR, "NOT_OPERATOR"},
        {RulesLexerToken::COLUMN, "COLUMN"},
        {RulesLexerToken::GOAL, "GOAL"},
        {RulesLexerToken::EPSILON, "EPSILON"},
        {RulesLexerToken::LEXER_BLOCK, "LEXER_BLOCK"},
        {RulesLexerToken::LEXER_BLOCK_DEFINITION, "LEXER_BLOCK_DEFINITION"},
        {RulesLexerToken::PARSER_BLOCK, "PARSER_BLOCK"},
        {RulesLexerToken::PARSER_BLOCK_DEFINITION, "PARSER_BLOCK_DEFINITION"},
        {RulesLexerToken::ANY_BLOCK, "ANY_BLOCK"},
        {RulesLexerToken::LEXER_RULE, "LEXER_RULE"},
        {RulesLexerToken::PARSER_RULE, "PARSER_RULE"},
        {RulesLexerToken::PARSER_RULE_BODY, "PARSER_RULE_BODY"},
        {RulesLexerToken::PARSER_RULE_DECL, "PARSER_RULE_DECL"},
        {RulesLexerToken::PARSER_RULE_ALTERNATIVE, "PARSER_RULE_ALTERNATIVE"},
        {RulesLexerToken::LEXER_RULE_BODY, "LEXER_RULE_BODY"},
        {RulesLexerToken::LEXER_RULE_BLOCK, "LEXER_RULE_BLOCK"},
        {RulesLexerToken::LEXER_RULE_DECL, "LEXER_RULE_DECL"},
        {RulesLexerToken::LEXER_RULE_OPTIONS, "LEXER_RULE_OPTIONS"},
        {RulesLexerToken::LEXER_RULE_ALTERNATIVE, "LEXER_RULE_ALTERNATIVE"},
    };
}// namespace ccl::parser::reader

namespace ccl::lexer
{
    template<>
    CCL_DECL auto lexerEnumToString<ccl::parser::reader::RulesLexerToken>(
        ccl::parser::reader::RulesLexerToken value) -> isl::string_view
    {
        return ccl::parser::reader::ToStringRulesLexerToken[value];
    }
}// namespace ccl::lexer

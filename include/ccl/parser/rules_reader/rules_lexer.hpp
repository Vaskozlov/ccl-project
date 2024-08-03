#pragma once

#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <isl/flatmap.hpp>

namespace ccl::parser::reader
{
    enum RulesLexerToken : ccl::u32;
}

template<>
CCL_DECL auto ccl::lexer::lexerEnumToString<ccl::parser::reader::RulesLexerToken>(
    ccl::parser::reader::RulesLexerToken value) -> isl::string_view;

namespace ccl::parser::reader
{
    enum RulesLexerToken : ccl::u32
    {
        EOI = 0ULL,
        BAD_TOKEN = 1ULL,
        CUT = 2ULL,
        LEXER_START = 3ULL,
        PARSER_START = 4ULL,
        OR = 5ULL,
        STAR = 6ULL,
        PLUS = 7ULL,
        EXCLAMATION_POINT = 8ULL,
        CURLY_OPEN = 9ULL,
        CURLY_CLOSE = 10ULL,
        COMMA = 11ULL,
        NUMBER = 12ULL,
        RULE_IDENTIFIER = 13ULL,
        IDENTIFIER = 14ULL,
        RULE_REFERENCE = 15ULL,
        STRING = 16ULL,
        ANY_PLACE_STRING = 17ULL,
        UNION = 18ULL,
        PREFIX_POSTFIX_OPERATOR = 19ULL,
        HIDE_OPERATOR = 20ULL,
        NOT_OPERATOR = 21ULL,
        COLUMN = 22ULL,
        GOAL = 23ULL,
        EPSILON = 24ULL,
        LEXER_BLOCK = 25ULL,
        LEXER_BLOCK_DEFINITION = 26ULL,
        PARSER_BLOCK = 27ULL,
        PARSER_BLOCK_DEFINITION = 28ULL,
        ANY_BLOCK = 29ULL,
        LEXER_RULE = 30ULL,
        PARSER_RULE = 31ULL,
        PARSER_RULE_BODY = 32ULL,
        PARSER_RULE_DECL = 33ULL,
        PARSER_RULE_ALTERNATIVE = 34ULL,
        LEXER_RULE_BODY = 35ULL,
        LEXER_RULE_BLOCK = 36ULL,
        LEXER_RULE_DECL = 37ULL,
        LEXER_RULE_OPTIONS = 38ULL,
        LEXER_RULE_ALTERNATIVE = 39ULL,
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
            {RulesLexerToken::ANY_PLACE_STRING, R"( ! ['] ([']^ | "\\\"")* ['] )"},
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

    inline constexpr isl::StaticFlatmap<ccl::SmallId, isl::string_view, 40> ToStringRulesLexerToken{
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
        {RulesLexerToken::ANY_PLACE_STRING, "ANY_PLACE_STRING"},
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

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
        EOI = 0,
        BAD_TOKEN = 1,
        CUT = 2,
        LEXER_START = 3,
        PARSER_START = 4,
        OR = 5,
        STAR = 6,
        PLUS = 7,
        EXCLAMATION_POINT = 8,
        CURLY_OPEN = 9,
        CURLY_CLOSE = 10,
        COMMA = 11,
        NUMBER = 12,
        RULE_IDENTIFIER = 13,
        IDENTIFIER = 14,
        RULE_REFERENCE = 15,
        STRING = 16,
        ANY_PLACE_STRING = 17,
        UNION = 18,
        PREFIX_POSTFIX_OPERATOR = 19,
        HIDE_OPERATOR = 20,
        NOT_OPERATOR = 21,
        COLUMN = 22,
        ANGLE_OPEN = 23,
        ANGLE_CLOSE = 24,
        GOAL = 25,
        EPSILON = 26,
        LEXER_BLOCK = 27,
        LEXER_BLOCK_DEFINITION = 28,
        PARSER_BLOCK = 29,
        PARSER_BLOCK_DEFINITION = 30,
        ANY_BLOCK = 31,
        LEXER_RULE = 32,
        PARSER_RULE = 33,
        PARSER_RULE_BODY = 34,
        PARSER_RULE_DECL = 35,
        PARSER_RULE_ALTERNATIVE = 36,
        LEXER_RULE_BODY = 37,
        LEXER_RULE_BLOCK = 38,
        LEXER_RULE_ALTERNATIVE = 39,
        LEXER_RULE_DECL = 40,
        LEXER_RULE_OPTIONS = 41,
    };

    // NOLINTNEXTLINE
    inline auto RulesLexer = ccl::lexer::LexicalAnalyzer{
        ccl::handler::Cmd::instance(),
        {
            {RulesLexerToken::LEXER_START, R"( "%LEXER%" )"},
            {RulesLexerToken::PARSER_START, R"( "%PARSER%" )"},
            {RulesLexerToken::OR, R"( ! [|] )"},
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
            {RulesLexerToken::STRING, R"( ! ["] ("\\\"" | ["]^)* ["] )"},
            {RulesLexerToken::ANY_PLACE_STRING, R"( ! ['] ([']^ | "\"")* ['] )"},
            {RulesLexerToken::UNION, R"( ! [\[] ("\\\]" | [\]]^)* [\]] )"},
            {RulesLexerToken::PREFIX_POSTFIX_OPERATOR, R"( ! [p] )"},
            {RulesLexerToken::HIDE_OPERATOR, R"( ! [h] )"},
            {RulesLexerToken::NOT_OPERATOR, R"( ! [^] )"},
            {RulesLexerToken::COLUMN, R"( ! [:] )"},
            {RulesLexerToken::ANGLE_OPEN, R"( ! [(] )"},
            {RulesLexerToken::ANGLE_CLOSE, R"( ! [)] )"},
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
            {RulesLexerToken::LEXER_RULE_ALTERNATIVE, R"( [#] )"},
            {RulesLexerToken::LEXER_RULE_DECL, R"( [#] )"},
            {RulesLexerToken::LEXER_RULE_OPTIONS, R"( [#] )"},
            {RulesLexerToken::LEXER_RULE_ALTERNATIVE, R"( [#] )"},
        }};

    inline constexpr isl::StaticFlatmap<ccl::SmallId, isl::string_view, 43> ToStringRulesLexerToken{
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
        {RulesLexerToken::ANGLE_OPEN, "ANGLE_OPEN"},
        {RulesLexerToken::ANGLE_CLOSE, "ANGLE_CLOSE"},
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
        {RulesLexerToken::LEXER_RULE_ALTERNATIVE, "LEXER_RULE_ALTERNATIVE"},
        {RulesLexerToken::LEXER_RULE_DECL, "LEXER_RULE_DECL"},
        {RulesLexerToken::LEXER_RULE_OPTIONS, "LEXER_RULE_OPTIONS"},
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

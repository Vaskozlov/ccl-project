#pragma once

#include <ccl/handler/cmd.hpp>
#include <ccl/lexer/tokenizer.hpp>
#include <isl/static_flat_map.hpp>

namespace ccl::parser::reader
{
    enum RulesLexerToken : ccl::SmallId;
}

template<>
CCL_DECL auto ccl::lexer::lexerEnumToString<ccl::parser::reader::RulesLexerToken>(
    ccl::parser::reader::RulesLexerToken value) -> std::string;

namespace ccl::parser::reader
{
    enum RulesLexerToken : ccl::SmallId
    {
        EOI = 0U,
        BAD_TOKEN = 1U,
        CUT = 2U,
        LEXER_START = 3U,
        PARSER_START = 4U,
        OR = 5U,
        STAR = 6U,
        PLUS = 7U,
        EXCLAMATION_POINT = 8U,
        CURLY_OPEN = 9U,
        CURLY_CLOSE = 10U,
        COMMA = 11U,
        NUMBER = 12U,
        RULE_IDENTIFIER = 13U,
        IDENTIFIER = 14U,
        RULE_REFERENCE = 15U,
        STRING = 16U,
        ANY_PLACE_STRING = 17U,
        UNION = 18U,
        PREFIX_POSTFIX_OPERATOR = 19U,
        HIDE_OPERATOR = 20U,
        NOT_OPERATOR = 21U,
        COLUMN = 22U,
        ANGLE_OPEN = 23U,
        ANGLE_CLOSE = 24U,
        GOAL = 25U,
        EPSILON = 26U,
        BLOCKS = 27U,
        LEXER_BLOCK = 28U,
        PARSER_BLOCK = 29U,
        LEXER_RULE = 30U,
        PARSER_RULE = 31U,
        PARSER_RULE_ALTERNATIVE = 32U,
        LEXER_RULE_BLOCK = 33U,
        LEXER_RULE_ALTERNATIVE = 34U,
        LEXER_RULE_OPTIONS = 35U,
    };

    // NOLINTNEXTLINE
    inline auto const RulesLexer = ccl::lexer::LexicalAnalyzer{
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
            {RulesLexerToken::BLOCKS, R"( [#] )"},
            {RulesLexerToken::LEXER_BLOCK, R"( [#] )"},
            {RulesLexerToken::PARSER_BLOCK, R"( [#] )"},
            {RulesLexerToken::LEXER_RULE, R"( [#] )"},
            {RulesLexerToken::PARSER_RULE, R"( [#] )"},
            {RulesLexerToken::PARSER_RULE_ALTERNATIVE, R"( [#] )"},
            {RulesLexerToken::LEXER_RULE_BLOCK, R"( [#] )"},
            {RulesLexerToken::LEXER_RULE_ALTERNATIVE, R"( [#] )"},
            {RulesLexerToken::LEXER_RULE_OPTIONS, R"( [#] )"},
        }};

    inline constexpr isl::StaticFlatmap<ccl::SmallId, isl::string_view, 36> ToStringRulesLexerToken{
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
        {RulesLexerToken::BLOCKS, "BLOCKS"},
        {RulesLexerToken::LEXER_BLOCK, "LEXER_BLOCK"},
        {RulesLexerToken::PARSER_BLOCK, "PARSER_BLOCK"},
        {RulesLexerToken::LEXER_RULE, "LEXER_RULE"},
        {RulesLexerToken::PARSER_RULE, "PARSER_RULE"},
        {RulesLexerToken::PARSER_RULE_ALTERNATIVE, "PARSER_RULE_ALTERNATIVE"},
        {RulesLexerToken::LEXER_RULE_BLOCK, "LEXER_RULE_BLOCK"},
        {RulesLexerToken::LEXER_RULE_ALTERNATIVE, "LEXER_RULE_ALTERNATIVE"},
        {RulesLexerToken::LEXER_RULE_OPTIONS, "LEXER_RULE_OPTIONS"},
    };
}// namespace ccl::parser::reader

namespace ccl::lexer
{
    template<>
    CCL_DECL auto lexerEnumToString<ccl::parser::reader::RulesLexerToken>(
        ccl::parser::reader::RulesLexerToken value) -> std::string
    {
        return static_cast<std::string>(ccl::parser::reader::ToStringRulesLexerToken[value]);
    }
}// namespace ccl::lexer

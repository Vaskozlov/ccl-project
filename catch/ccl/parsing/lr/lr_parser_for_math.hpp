#ifndef CCL_PROJECT_LR_PARSER_FOR_MATH_HPP
#define CCL_PROJECT_LR_PARSER_FOR_MATH_HPP

#include <ccl/parser/lr/lr_parser.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>

namespace ccl::debug
{
    constexpr inline isl::string_view SimpleLrGrammarDefinition = R"(
%PARSER%
GOAL:
    EXPR

EXPR:
    EXPR '+' FACTOR
    | FACTOR

FACTOR:
    NUMBER
    | '(' EXPR ')'

%LEXER%
NUMBER:     [0-9]+
)";

    constexpr inline isl::string_view SimpleGlrMathGrammarDefinition = R"(
%PARSER%
GOAL:
    EXPR

EXPR:
    EXPR '+' TERM
    | TERM

TERM:
    TERM '*' TERM
    | NUMBER

%LEXER%
NUMBER:     [0-9]+
)";

    constexpr inline isl::string_view MathGrammarDefinition = R"(
%PARSER%
GOAL:
    EXPR

EXPR:
    EXPR '+' TERM
    | TERM

TERM:
    TERM '*' VALUE
    | VALUE

VALUE:
    FACTOR
    | '-' VALUE

FACTOR:
    NUMBER
    | '(' EXPR ')'

%LEXER%
NUMBER:     [0-9]+
IDENTIFIER:  [a-zA-Z_][a-zA-Z0-9_]*
)";
}// namespace ccl::debug

#endif /* CCL_PROJECT_LR_PARSER_FOR_MATH_HPP */

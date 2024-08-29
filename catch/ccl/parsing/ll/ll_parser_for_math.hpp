#ifndef CCL_PROJECT_LL_PARSER_FOR_MATH_HPP
#define CCL_PROJECT_LL_PARSER_FOR_MATH_HPP

#include <ccl/parser/rules_reader/rules_reader.hpp>

namespace ccl::debug
{
    constexpr inline isl::string_view SimpleLlGrammarDefinition = R"(
%PARSER%
GOAL:
    EXPR

EXPR:
    EXPR '+' TERM
    | NUMBER

TERM:
    TERM '*' NUMBER
    | NUMBER

%LEXER%
NUMBER:     [0-9]+
)";
}// namespace ccl::debug

#endif /* CCL_PROJECT_LL_PARSER_FOR_MATH_HPP */

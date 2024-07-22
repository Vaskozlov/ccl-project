#ifndef CCL_PROJECT_LR_PARSER_FOR_MATH_HPP
#define CCL_PROJECT_LR_PARSER_FOR_MATH_HPP

#include "basic_math_grammar.hpp"
#include <ccl/parser/lr/lr_parser.hpp>

namespace ccl::debug
{
    const inline ccl::parser::GrammarRulesStorage MathRules{
        {GrammarSymbol::GOAL,
         {
             ccl::parser::Rule{
                 {
                     GrammarSymbol::EXPR,
                 },
             },
         }},
        {GrammarSymbol::EXPR,
         {
             ccl::parser::Rule{
                 {
                     GrammarSymbol::EXPR,
                     GrammarSymbol::ADD,
                     GrammarSymbol::TERM,
                 },
             },
             ccl::parser::Rule{
                 {
                     GrammarSymbol::TERM,
                 },
             },
         }},
        {GrammarSymbol::TERM,
         {
             ccl::parser::Rule{
                 {
                     GrammarSymbol::TERM,
                     GrammarSymbol::MUL,
                     GrammarSymbol::VALUE,
                 },
             },
             ccl::parser::Rule{
                 {
                     GrammarSymbol::VALUE,
                 },
             },
         }},
        {GrammarSymbol::VALUE,
         {
             ccl::parser::Rule{
                 {
                     GrammarSymbol::FACTOR,
                 },
             },
             ccl::parser::Rule{
                 {
                     GrammarSymbol::SUB,
                     GrammarSymbol::VALUE,
                 },
             },
         }},
        {GrammarSymbol::FACTOR,
         {
             ccl::parser::Rule{
                 {
                     GrammarSymbol::NAME,
                 },
             },
             ccl::parser::Rule{
                 {
                     GrammarSymbol::NUMBER,
                 },
             },
             ccl::parser::Rule{
                 {
                     GrammarSymbol::ANGLE_OPEN,
                     GrammarSymbol::EXPR,
                     GrammarSymbol::ANGLE_CLOSE,
                 },
             },
         }}};

    const inline parser::LrItem MathStartItem = parser::LrItem{
        ccl::parser::Rule{{GrammarSymbol::EXPR}}, 0, GrammarSymbol::GOAL, GrammarSymbol::EOI};

    const inline parser::LrParser MathParser{MathStartItem, GrammarSymbol::EPSILON, MathRules};
}// namespace ccl::debug

#endif /* CCL_PROJECT_LR_PARSER_FOR_MATH_HPP */

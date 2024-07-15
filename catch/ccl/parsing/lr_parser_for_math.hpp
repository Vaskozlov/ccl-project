#ifndef CCL_PROJECT_LR_PARSER_FOR_MATH_HPP
#define CCL_PROJECT_LR_PARSER_FOR_MATH_HPP

#include "basic_math_grammar.hpp"
#include <ccl/parser/lr/lr_parser.hpp>

namespace ccl::debug
{
    const inline ccl::parser::GrammarRulesStorage MathRules{
        {GrammarSymbol::GOAL,
         {
             {GrammarSymbol::EXPR},
         }},
        {GrammarSymbol::EXPR,
         {
             {GrammarSymbol::EXPR, GrammarSymbol::ADD, GrammarSymbol::TERM},
             {GrammarSymbol::TERM},
         }},
        {GrammarSymbol::TERM,
         {
             {GrammarSymbol::TERM, GrammarSymbol::MUL, GrammarSymbol::VALUE},
             {GrammarSymbol::VALUE},
         }},
        {GrammarSymbol::VALUE,
         {
             {GrammarSymbol::FACTOR},
             {GrammarSymbol::SUB, GrammarSymbol::VALUE},
         }},
        {GrammarSymbol::FACTOR,
         {
             {GrammarSymbol::NAME},
             {GrammarSymbol::NUMBER},
             {GrammarSymbol::ANGLE_OPEN, GrammarSymbol::EXPR, GrammarSymbol::ANGLE_CLOSE},
         }}};

    const inline parser::LrItem MathStartItem =
        parser::LrItem{{GrammarSymbol::EXPR}, 0, GrammarSymbol::GOAL, GrammarSymbol::EOI};

    const inline parser::LrParser MathParser{
        MathStartItem, GrammarSymbol::EPSILON, AllSymbols, Terminals, MathRules};
}// namespace ccl::debug

#endif /* CCL_PROJECT_LR_PARSER_FOR_MATH_HPP */

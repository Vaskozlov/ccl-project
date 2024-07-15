#ifndef CCL_PROJECT_TEST_GRAMMAR_HPP
#define CCL_PROJECT_TEST_GRAMMAR_HPP

#include "ff_enum_helper.hpp"
#include <ccl/parser/grammar_rules_storage.hpp>

namespace ccl::debug
{
    using GrammarSymbol = ff_test::helperToken;

    const inline parser::GrammarRulesStorage Rules{
        {GrammarSymbol::GOAL,
         {
             {GrammarSymbol::EXPR, GrammarSymbol::EOI},
         }},
        {GrammarSymbol::EXPR,
         {
             {GrammarSymbol::TERM, GrammarSymbol::EXPR_C},
         }},
        {GrammarSymbol::EXPR_C,
         {
             {GrammarSymbol::ADD, GrammarSymbol::TERM, GrammarSymbol::EXPR_C},
             {GrammarSymbol::SUB, GrammarSymbol::TERM, GrammarSymbol::EXPR_C},
             {GrammarSymbol::EPSILON},
         }},
        {GrammarSymbol::TERM,
         {
             {GrammarSymbol::FACTOR, GrammarSymbol::TERM_C},
         }},
        {GrammarSymbol::TERM_C,
         {
             {GrammarSymbol::MUL, GrammarSymbol::FACTOR, GrammarSymbol::TERM_C},
             {GrammarSymbol::DIV, GrammarSymbol::FACTOR, GrammarSymbol::TERM_C},
             {GrammarSymbol::EPSILON},
         }},
        {GrammarSymbol::FACTOR,
         {
             {GrammarSymbol::ANGLE_OPEN, GrammarSymbol::EXPR, GrammarSymbol::ANGLE_CLOSE},
             {GrammarSymbol::NAME},
             {GrammarSymbol::NUMBER},
         }}};

    const inline isl::Set<Id> Terminals{
        GrammarSymbol::NAME,       GrammarSymbol::NUMBER, GrammarSymbol::ANGLE_CLOSE,
        GrammarSymbol::ANGLE_OPEN, GrammarSymbol::ADD,    GrammarSymbol::SUB,
        GrammarSymbol::MUL,        GrammarSymbol::DIV,    GrammarSymbol::EPSILON,
        GrammarSymbol::EOI,
    };

    const inline isl::Set<Id> AllSymbols = []() {
        isl::Set<Id> result;

        for (const auto &[token, repr] : ff_test::ToStringhelperToken) {
            if (lexer::isUsedDefinedTokenOrEoF(token)) {
                result.emplace(token);
            }
        }

        return result;
    }();
}// namespace ccl::debug

template<>
class fmt::formatter<ccl::debug::GrammarSymbol> : public fmt::formatter<std::string_view>
{
public:
    template<typename FmtContext>
    constexpr auto format(const ccl::debug::GrammarSymbol &symbol, FmtContext &ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", ccl::lexer::lexerEnumToString(symbol));
    }
};

#endif /* CCL_PROJECT_TEST_GRAMMAR_HPP */

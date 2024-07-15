#include "ccl/parser/lr/lr_parser.hpp"
#include "ff_enum_helper.hpp"
#include <ccl/lexer/dot_item/container.hpp>

using namespace ccl::lexer;

using GrammarSymbol = ff_test::helperToken;

template<>
class fmt::formatter<GrammarSymbol> : public fmt::formatter<std::string_view>
{
public:
    template<typename FmtContext>
    constexpr auto format(const GrammarSymbol &action, FmtContext &ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", ccl::lexer::lexerEnumToString(action));
    }
};

const inline ccl::parser::GrammarRulesStorage Rules{
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

const inline isl::Set<ccl::Id> Terminals{
    GrammarSymbol::NAME,       GrammarSymbol::NUMBER, GrammarSymbol::ANGLE_CLOSE,
    GrammarSymbol::ANGLE_OPEN, GrammarSymbol::ADD,    GrammarSymbol::SUB,
    GrammarSymbol::MUL,        GrammarSymbol::DIV,    GrammarSymbol::EPSILON,
    GrammarSymbol::EOI,
};

const inline isl::Set<ccl::Id> AllSymbols = []() {
    isl::Set<ccl::Id> result;

    for (const auto &[token, repr] : ff_test::ToStringhelperToken) {
        result.emplace(token);
    }

    return result;
}();

auto main() -> int
{
    const auto start_lr_item_t =
        ccl::parser::LrItem{{GrammarSymbol::EXPR}, 0, GrammarSymbol::GOAL, GrammarSymbol::EOI};

    auto parser = ccl::parser::LrParser(
        start_lr_item_t, GrammarSymbol::EPSILON, AllSymbols, Terminals, Rules);

    auto tokenizer = ff_test::helper.getTokenizer("1*2*(3+-4)");

    parser.parse(tokenizer)->print("", false, [](auto arg) {
        return lexerEnumToString(isl::as<GrammarSymbol>(arg));
    });

    return 0;
}

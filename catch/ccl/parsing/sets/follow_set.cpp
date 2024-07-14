#include "test_grammar.hpp"
#include <ccl/debug/debug_file.hpp>
#include <ccl/parser/first_set.hpp>
#include <ccl/parser/follow_set.hpp>

TEST_CASE("FollowSetComputation", "[FollowSet]")
{
    using namespace isl;
    using namespace ccl;
    using namespace ccl::debug;
    using namespace ccl::parser;

    auto first_set = evaluateFirstSet(GrammarSymbol::EPSILON, AllSymbols, Terminals, Rules);

    auto follow_Set = evaluateFollowSet(
        GrammarSymbol::GOAL, GrammarSymbol::EOI, GrammarSymbol::EPSILON, AllSymbols, Terminals,
        Rules, first_set);

    const auto &goal_follow_set = follow_Set.at(GrammarSymbol::GOAL);
    const auto &expr_follow_set = follow_Set.at(GrammarSymbol::EXPR);
    const auto &expr_c_follow_set = follow_Set.at(GrammarSymbol::EXPR_C);
    const auto &term_follow_set = follow_Set.at(GrammarSymbol::TERM);
    const auto &term_c_follow_set = follow_Set.at(GrammarSymbol::TERM_C);
    const auto &factor_follow_set = follow_Set.at(GrammarSymbol::FACTOR);

    REQUIRE(
        goal_follow_set == isl::Set<Production>{
                               GrammarSymbol::EOI,
                           });

    REQUIRE(
        expr_follow_set == isl::Set<Production>{
                               GrammarSymbol::EOI,
                               GrammarSymbol::ANGLE_CLOSE,
                           });

    REQUIRE(expr_c_follow_set == expr_follow_set);

    REQUIRE(
        term_follow_set == isl::Set<Production>{
                               GrammarSymbol::EOI,
                               GrammarSymbol::ANGLE_CLOSE,
                               GrammarSymbol::ADD,
                               GrammarSymbol::SUB,
                           });

    REQUIRE(term_c_follow_set == term_follow_set);

    REQUIRE(
        factor_follow_set == isl::Set<Production>{
                                 GrammarSymbol::EOI,
                                 GrammarSymbol::ANGLE_CLOSE,
                                 GrammarSymbol::ADD,
                                 GrammarSymbol::SUB,
                                 GrammarSymbol::MUL,
                                 GrammarSymbol::DIV,
                             });
}

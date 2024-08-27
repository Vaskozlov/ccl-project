#include "basic_math_grammar.hpp"
#include <ccl/debug/debug.hpp>
#include <ccl/parser/first_set.hpp>
#include <ccl/parser/follow_set.hpp>

TEST_CASE("FollowSetComputation", "[FollowSet]")
{
    using namespace isl;
    using namespace ccl;
    using namespace ccl::debug;
    using namespace ccl::parser;

    const auto &follow_Set = GrammarForFirstFollowSetTest.getFollowSet();

    const auto &goal_follow_set = follow_Set.at(GrammarSymbol::GOAL);
    const auto &expr_follow_set = follow_Set.at(GrammarSymbol::EXPR);
    const auto &expr_c_follow_set = follow_Set.at(GrammarSymbol::EXPR_C);
    const auto &term_follow_set = follow_Set.at(GrammarSymbol::TERM);
    const auto &term_c_follow_set = follow_Set.at(GrammarSymbol::TERM_C);
    const auto &factor_follow_set = follow_Set.at(GrammarSymbol::FACTOR);

    REQUIRE(
        goal_follow_set == std::unordered_set<Symbol>{
                               GrammarSymbol::EOI,
                           });

    REQUIRE(
        expr_follow_set == std::unordered_set<Symbol>{
                               GrammarSymbol::EOI,
                               GrammarSymbol::ANGLE_CLOSE,
                           });

    REQUIRE(expr_c_follow_set == expr_follow_set);

    REQUIRE(
        term_follow_set == std::unordered_set<Symbol>{
                               GrammarSymbol::EOI,
                               GrammarSymbol::ANGLE_CLOSE,
                               GrammarSymbol::ADD,
                               GrammarSymbol::SUB,
                           });

    REQUIRE(term_c_follow_set == term_follow_set);

    REQUIRE(
        factor_follow_set == std::unordered_set<Symbol>{
                                 GrammarSymbol::EOI,
                                 GrammarSymbol::ANGLE_CLOSE,
                                 GrammarSymbol::ADD,
                                 GrammarSymbol::SUB,
                                 GrammarSymbol::MUL,
                                 GrammarSymbol::DIV,
                             });
}

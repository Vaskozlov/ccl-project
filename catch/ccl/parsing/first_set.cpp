#include "basic_math_grammar.hpp"
#include <ccl/debug/debug.hpp>
#include <ccl/parser/first_set.hpp>

TEST_CASE("FirstSetComputation", "[FirstSet]")
{
    using namespace isl;
    using namespace ccl;
    using namespace ccl::debug;
    using namespace ccl::parser;

    const auto &first_set = GrammarForFirstFollowSetTest.getFirstSet();

    const auto &goal_first_set = first_set.at(GrammarSymbol::GOAL);
    const auto &expr_first_set = first_set.at(GrammarSymbol::EXPR);
    const auto &expr_c_first_set = first_set.at(GrammarSymbol::EXPR_C);
    const auto &term_first_set = first_set.at(GrammarSymbol::TERM);
    const auto &term_c_first_set = first_set.at(GrammarSymbol::TERM_C);
    const auto &factor_first_set = first_set.at(GrammarSymbol::FACTOR);

    REQUIRE(
        goal_first_set == std::unordered_set<Symbol>{
                              GrammarSymbol::ANGLE_OPEN,
                              GrammarSymbol::NUMBER,
                              GrammarSymbol::NAME,
                          });

    REQUIRE(expr_first_set == goal_first_set);

    REQUIRE(
        expr_c_first_set == std::unordered_set<Symbol>{
                                GrammarSymbol::ADD,
                                GrammarSymbol::SUB,
                                GrammarSymbol::EPSILON,
                            });

    REQUIRE(term_first_set == goal_first_set);

    REQUIRE(
        term_c_first_set == std::unordered_set<Symbol>{
                                GrammarSymbol::MUL,
                                GrammarSymbol::DIV,
                                GrammarSymbol::EPSILON,
                            });

    REQUIRE(factor_first_set == goal_first_set);
}

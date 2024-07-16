#include "basic_math_grammar.hpp"
#include "lr_parser_for_math.hpp"
#include <ccl/debug/debug.hpp>
#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>

TEST_CASE("TreeStructureTest", "[TreeParsing]")
{
    using namespace ccl::debug;
    using namespace ccl::parser;

    auto tokenizer = math::MathLexer.getTokenizer("10+2*3");
    auto parse_result = MathParser.parse(tokenizer);
    const auto *root_node = isl::as<ast::UnNodeSequence *>(parse_result.get());

    REQUIRE(parse_result != nullptr);
    REQUIRE(root_node->getType() == GrammarSymbol::EXPR);

    // Left EXPR
    const auto *left_expr_node = isl::as<const ast::UnNodeSequence *>(root_node->at(0));
    REQUIRE(left_expr_node->getType() == GrammarSymbol::EXPR);

    const auto *term_node = isl::as<const ast::UnNodeSequence *>(left_expr_node->at(0));
    REQUIRE(term_node->getType() == GrammarSymbol::TERM);

    const auto *value_node = isl::as<const ast::UnNodeSequence *>(term_node->at(0));
    REQUIRE(value_node->getType() == GrammarSymbol::VALUE);

    const auto *factor_node = isl::as<const ast::UnNodeSequence *>(value_node->at(0));

    REQUIRE(factor_node->getType() == GrammarSymbol::FACTOR);
    const auto *number_10_node = isl::as<const ast::TokenNode *>(factor_node->at(0));
    REQUIRE(number_10_node->getToken().getRepr() == "10");

    // ADD operation
    const auto *add_op_node = isl::as<const ast::TokenNode *>(root_node->at(1));
    REQUIRE(add_op_node->getToken().getRepr() == "+");

    // Right TERM
    const auto *right_term_node = isl::as<const ast::UnNodeSequence *>(root_node->at(2));
    REQUIRE(right_term_node->getType() == GrammarSymbol::TERM);

    const auto *nested_term_node = isl::as<const ast::UnNodeSequence *>(right_term_node->at(0));
    REQUIRE(nested_term_node->getType() == GrammarSymbol::TERM);

    const auto *nested_value_node = isl::as<const ast::UnNodeSequence *>(nested_term_node->at(0));
    REQUIRE(nested_value_node->getType() == GrammarSymbol::VALUE);

    const auto *nested_factor_node = isl::as<const ast::UnNodeSequence *>(nested_value_node->at(0));
    REQUIRE(nested_factor_node->getType() == GrammarSymbol::FACTOR);

    const auto *number_2_node = isl::as<const ast::TokenNode *>(nested_factor_node->at(0));
    REQUIRE(number_2_node->getToken().getRepr() == "2");

    // MUL operation
    const auto *mul_op_node = isl::as<const ast::TokenNode *>(right_term_node->at(1));
    REQUIRE(mul_op_node->getToken().getRepr() == "*");

    // Right VALUE
    const auto *right_value_node = isl::as<const ast::UnNodeSequence *>(right_term_node->at(2));
    REQUIRE(right_value_node->getType() == GrammarSymbol::VALUE);

    const auto *right_nested_factor_node =
        isl::as<const ast::UnNodeSequence *>(right_value_node->at(0));
    REQUIRE(right_nested_factor_node->getType() == GrammarSymbol::FACTOR);

    const auto *number3 = isl::as<const ast::TokenNode *>(right_nested_factor_node->at(0));
    REQUIRE(number3->getToken().getRepr() == "3");
}
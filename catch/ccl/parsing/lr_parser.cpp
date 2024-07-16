#include "basic_math_grammar.hpp"
#include "lr_parser_for_math.hpp"
#include <ccl/debug/debug.hpp>
#include <ccl/parser/ast/node_sequence.hpp>
#include <ccl/parser/ast/token_node.hpp>

TEST_CASE("TreeStructureTest", "[TreeParsing]")
{
    using namespace ccl::debug;

    auto tokenizer = math::MathLexer.getTokenizer("10+2*3");
    auto parse_result = MathParser.parse(tokenizer);
    const auto *root_node = isl::as<ccl::parser::ast::NodeSequence *>(parse_result.get());

    REQUIRE(parse_result != nullptr);
    REQUIRE(root_node->getType() == GrammarSymbol::EXPR);

    // Left EXPR
    const auto *left_expr_node =
        isl::as<const ccl::parser::ast::NodeSequence *>(root_node->at(0).value());
    REQUIRE(left_expr_node->getType() == GrammarSymbol::EXPR);

    const auto *term_node =
        isl::as<const ccl::parser::ast::NodeSequence *>(left_expr_node->at(0).value());
    REQUIRE(term_node->getType() == GrammarSymbol::TERM);

    const auto *value_node =
        isl::as<const ccl::parser::ast::NodeSequence *>(term_node->at(0).value());
    REQUIRE(value_node->getType() == GrammarSymbol::VALUE);

    const auto *factor_node =
        isl::as<const ccl::parser::ast::NodeSequence *>(value_node->at(0).value());

    REQUIRE(factor_node->getType() == GrammarSymbol::FACTOR);
    const auto *number_10_node =
        isl::as<const ccl::parser::ast::TokenNode *>(factor_node->at(0).value());
    REQUIRE(number_10_node->getToken().getRepr() == "10");

    // ADD operation
    const auto *add_op_node =
        isl::as<const ccl::parser::ast::TokenNode *>(root_node->at(1).value());
    REQUIRE(add_op_node->getToken().getRepr() == "+");

    // Right TERM
    const auto *right_term_node =
        isl::as<const ccl::parser::ast::NodeSequence *>(root_node->at(2).value());
    REQUIRE(right_term_node->getType() == GrammarSymbol::TERM);

    const auto *nested_term_node =
        isl::as<const ccl::parser::ast::NodeSequence *>(right_term_node->at(0).value());
    REQUIRE(nested_term_node->getType() == GrammarSymbol::TERM);

    const auto *nested_value_node =
        isl::as<const ccl::parser::ast::NodeSequence *>(nested_term_node->at(0).value());
    REQUIRE(nested_value_node->getType() == GrammarSymbol::VALUE);

    const auto *nested_factor_node =
        isl::as<const ccl::parser::ast::NodeSequence *>(nested_value_node->at(0).value());
    REQUIRE(nested_factor_node->getType() == GrammarSymbol::FACTOR);

    const auto *number_2_node =
        isl::as<const ccl::parser::ast::TokenNode *>(nested_factor_node->at(0).value());
    REQUIRE(number_2_node->getToken().getRepr() == "2");

    // MUL operation
    const auto *mul_op_node =
        isl::as<const ccl::parser::ast::TokenNode *>(right_term_node->at(1).value());
    REQUIRE(mul_op_node->getToken().getRepr() == "*");

    // Right VALUE
    const auto *right_value_node =
        isl::as<const ccl::parser::ast::NodeSequence *>(right_term_node->at(2).value());
    REQUIRE(right_value_node->getType() == GrammarSymbol::VALUE);

    const auto *right_nested_factor_node =
        isl::as<const ccl::parser::ast::NodeSequence *>(right_value_node->at(0).value());
    REQUIRE(right_nested_factor_node->getType() == GrammarSymbol::FACTOR);

    const auto *number3 =
        isl::as<const ccl::parser::ast::TokenNode *>(right_nested_factor_node->at(0).value());
    REQUIRE(number3->getToken().getRepr() == "3");
}
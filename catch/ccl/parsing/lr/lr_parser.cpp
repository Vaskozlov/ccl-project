#include "ccl/debug/debug.hpp"
#include "ccl/parser/ast/token_node.hpp"
#include "lr_parser_for_math.hpp"

TEST_CASE("TreeStructureTest", "[TreeParsing]")
{
    using namespace ccl::debug;
    using namespace ccl::parser;

    reader::RulesReader math_rules_reader{MathGrammarDefinition, __FILE__};
    auto &math_rules_constructor = math_rules_reader.getParserBuilder();
    const auto &lexer = math_rules_constructor.getLexicalAnalyzer();
    auto math_token_to_string = math_rules_constructor.getIdToNameTranslationFunction();
    auto MathLrParser = math_rules_constructor.buildLr1();

    auto tokenizer = lexer.getTokenizer("10+2*3");
    auto [nodes_lifetime_manager, parse_result] = MathLrParser.parse(tokenizer);
    const auto root_node = isl::dynamicPointerCast<ast::NodeOfNodes>(parse_result);

    REQUIRE(parse_result != nullptr);
    REQUIRE(root_node->getType() == math_rules_constructor.getRuleId("EXPR"));

    // Left EXPR
    const auto *left_expr_node = isl::as<const ast::NodeOfNodes *>(root_node->at(0).get());
    REQUIRE(left_expr_node->getType() == math_rules_constructor.getRuleId("EXPR"));

    const auto *term_node = isl::as<const ast::NodeOfNodes *>(left_expr_node->at(0).get());
    REQUIRE(term_node->getType() == math_rules_constructor.getRuleId("TERM"));

    const auto *value_node = isl::as<const ast::NodeOfNodes *>(term_node->at(0).get());
    REQUIRE(value_node->getType() == math_rules_constructor.getRuleId("VALUE"));

    const auto *factor_node = isl::as<const ast::NodeOfNodes *>(value_node->at(0).get());

    REQUIRE(factor_node->getType() == math_rules_constructor.getRuleId("FACTOR"));
    const auto *number_10_node = isl::as<const ast::TokenNode *>(factor_node->at(0).get());
    REQUIRE(number_10_node->getToken().getRepr() == "10");

    // ADD operation
    const auto *add_op_node = isl::as<const ast::TokenNode *>(root_node->at(1).get());
    REQUIRE(add_op_node->getToken().getRepr() == "+");

    // Right TERM
    const auto *right_term_node = isl::as<const ast::NodeOfNodes *>(root_node->at(2).get());
    REQUIRE(right_term_node->getType() == math_rules_constructor.getRuleId("TERM"));

    const auto *nested_term_node = isl::as<const ast::NodeOfNodes *>(right_term_node->at(0).get());
    REQUIRE(nested_term_node->getType() == math_rules_constructor.getRuleId("TERM"));

    const auto *nested_value_node = isl::as<const ast::NodeOfNodes *>(nested_term_node->at(0).get());
    REQUIRE(nested_value_node->getType() == math_rules_constructor.getRuleId("VALUE"));

    const auto *nested_factor_node = isl::as<const ast::NodeOfNodes *>(nested_value_node->at(0).get());
    REQUIRE(nested_factor_node->getType() == math_rules_constructor.getRuleId("FACTOR"));

    const auto *number_2_node = isl::as<const ast::TokenNode *>(nested_factor_node->at(0).get());
    REQUIRE(number_2_node->getToken().getRepr() == "2");

    // MUL operation
    const auto *mul_op_node = isl::as<const ast::TokenNode *>(right_term_node->at(1).get());
    REQUIRE(mul_op_node->getToken().getRepr() == "*");

    // Right VALUE
    const auto *right_value_node = isl::as<const ast::NodeOfNodes *>(right_term_node->at(2).get());
    REQUIRE(right_value_node->getType() == math_rules_constructor.getRuleId("VALUE"));

    const auto *right_nested_factor_node =
        isl::as<const ast::NodeOfNodes *>(right_value_node->at(0).get());
    REQUIRE(right_nested_factor_node->getType() == math_rules_constructor.getRuleId("FACTOR"));

    const auto *number3 = isl::as<const ast::TokenNode *>(right_nested_factor_node->at(0).get());
    REQUIRE(number3->getToken().getRepr() == "3");
}
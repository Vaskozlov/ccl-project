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
    auto math_lr_parser = math_rules_constructor.buildLr1();

    auto tokenizer = lexer.getTokenizer("10+2*3");
    auto [parse_result, algorithm_name] = math_lr_parser.parse(tokenizer);
    const auto root_node = isl::dynamicPointerCast<ast::NonTerminal>(parse_result);

    REQUIRE(parse_result != nullptr);
    REQUIRE(root_node->getType() == math_rules_constructor.getRuleId("EXPR"));

    // Left EXPR
    const auto *left_expr_node = dynamic_cast<const ast::NonTerminal *>(root_node->at(0).get());
    REQUIRE(left_expr_node->getType() == math_rules_constructor.getRuleId("EXPR"));

    const auto *term_node = dynamic_cast<const ast::NonTerminal *>(left_expr_node->at(0).get());
    REQUIRE(term_node->getType() == math_rules_constructor.getRuleId("TERM"));

    const auto *value_node = dynamic_cast<const ast::NonTerminal *>(term_node->at(0).get());
    REQUIRE(value_node->getType() == math_rules_constructor.getRuleId("VALUE"));

    const auto *factor_node = dynamic_cast<const ast::NonTerminal *>(value_node->at(0).get());

    REQUIRE(factor_node->getType() == math_rules_constructor.getRuleId("FACTOR"));
    const auto *number_10_node = dynamic_cast<const ast::Terminal *>(factor_node->at(0).get());
    REQUIRE(number_10_node->getToken().getRepr() == "10");

    // ADD operation
    const auto *add_op_node = dynamic_cast<const ast::Terminal *>(root_node->at(1).get());
    REQUIRE(add_op_node->getToken().getRepr() == "+");

    // Right TERM
    const auto *right_term_node = dynamic_cast<const ast::NonTerminal *>(root_node->at(2).get());
    REQUIRE(right_term_node->getType() == math_rules_constructor.getRuleId("TERM"));

    const auto *nested_term_node =
        dynamic_cast<const ast::NonTerminal *>(right_term_node->at(0).get());
    REQUIRE(nested_term_node->getType() == math_rules_constructor.getRuleId("TERM"));

    const auto *nested_value_node =
        dynamic_cast<const ast::NonTerminal *>(nested_term_node->at(0).get());
    REQUIRE(nested_value_node->getType() == math_rules_constructor.getRuleId("VALUE"));

    const auto *nested_factor_node =
        dynamic_cast<const ast::NonTerminal *>(nested_value_node->at(0).get());
    REQUIRE(nested_factor_node->getType() == math_rules_constructor.getRuleId("FACTOR"));

    const auto *number_2_node =
        dynamic_cast<const ast::Terminal *>(nested_factor_node->at(0).get());
    REQUIRE(number_2_node->getToken().getRepr() == "2");

    // MUL operation
    const auto *mul_op_node = dynamic_cast<const ast::Terminal *>(right_term_node->at(1).get());
    REQUIRE(mul_op_node->getToken().getRepr() == "*");

    // Right VALUE
    const auto *right_value_node =
        dynamic_cast<const ast::NonTerminal *>(right_term_node->at(2).get());
    REQUIRE(right_value_node->getType() == math_rules_constructor.getRuleId("VALUE"));

    const auto *right_nested_factor_node =
        dynamic_cast<const ast::NonTerminal *>(right_value_node->at(0).get());
    REQUIRE(right_nested_factor_node->getType() == math_rules_constructor.getRuleId("FACTOR"));

    const auto *number3 =
        dynamic_cast<const ast::Terminal *>(right_nested_factor_node->at(0).get());
    REQUIRE(number3->getToken().getRepr() == "3");
}
#include "ccl/parser/lr/glr_parser.hpp"
#include "ccl/debug/debug.hpp"
#include "ccl/parser/ast/token_node.hpp"
#include "ccl/parser/dot/dot_repr.hpp"
#include "lr_parser_for_math.hpp"

TEST_CASE("GlrParser", "[TreeParsing]")
{
    using namespace ccl::debug;
    using namespace ccl::parser;

    reader::RulesReader math_rules_reader{SimpleGlrMathGrammarDefinition, __FILE__};
    auto &math_rules_constructor = math_rules_reader.getParserBuilder();
    const auto &lexer = math_rules_constructor.getLexicalAnalyzer();
    auto math_token_to_string = math_rules_constructor.getIdToNameTranslationFunction();

    auto MathLrParser = math_rules_constructor.buildGlr();

    auto tokenizer = lexer.getTokenizer("1*2*3+4");
    auto parse_result = MathLrParser.parse(tokenizer);

    for (const auto &elem : parse_result.getHead()) {
        elem->value->print("", false, math_token_to_string);
    }

    auto conversion_range = std::views::transform(parse_result.getHead(), [](auto &node) {
        return node->value.get();
    });

    auto tree_repr = dot::createDotRepresentation(
        std::vector<const ast::Node *>{conversion_range.begin(), conversion_range.end()},
        math_token_to_string);

    isl::io::writeToFile(std::filesystem::current_path().append("glr.dot"), tree_repr);
}
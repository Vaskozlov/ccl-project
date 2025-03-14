#include "ccl/parser/lr/glr_parser.hpp"
#include "ccl/parser/dot/dot_repr.hpp"
#include "lr_parser_for_math.hpp"
#include "ccl/debug/debug.hpp"
#include "ccl/lexer/tokenizer.hpp"
#include <isl/io.hpp>

TEST_CASE("GlrParser", "[GLR]")
{
    using namespace ccl::debug;
    using namespace ccl::parser;

    reader::RulesReader math_rules_reader{{}, SimpleGlrMathGrammarDefinition, __FILE__};
    auto &math_rules_constructor = math_rules_reader.getParserBuilder();
    const auto &lexer = math_rules_constructor.getLexicalAnalyzer();
    const auto math_token_to_string = math_rules_constructor.getIdToNameTranslationFunction();
    const auto math_lr_parser = math_rules_constructor.buildGlr();

    auto tokenizer = lexer.getTokenizer("1*2*3+4+5");
    auto [roots, algorithm_name] = math_lr_parser.parse(tokenizer);
    auto converter = std::views::transform(roots, [](auto &node) {
        return node.get();
    });

    auto tree_repr = dot::createDotRepresentation(
        std::vector<ast::Node *>{converter.begin(), converter.end()}, math_token_to_string);
}
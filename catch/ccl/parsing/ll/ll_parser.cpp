#include "ll_parser_for_math.hpp"
#include <ccl/parser/dot/dot_repr.hpp>
#include <ccl/parser/ll/gll.hpp>
#include <ccl/parser/ll/ll_1_parser.hpp>
#include <isl/io.hpp>

TEST_CASE("LLParser", "[LLParser]")
{
    // 14:22
    using namespace ccl::debug;
    using namespace ccl::parser;

    reader::RulesReader math_rules_reader{SimpleLlGrammarDefinition, __FILE__};
    auto &math_rules_constructor = math_rules_reader.getParserBuilder();
    const auto &lexer = math_rules_constructor.getLexicalAnalyzer();
    auto parser = math_rules_constructor.buildGLL();

    auto tokenizer = lexer.getTokenizer("1+2+3");
    auto [lifetime_manager, roots] = parser.parse(tokenizer);
    auto converter = std::views::transform(roots, [](auto &node) {
        return node.get();
    });

    auto tree_repr = dot::createDotRepresentation(
        std::vector<ast::Node *>{converter.begin(), converter.end()},
        math_rules_constructor.getIdToNameTranslationFunction());

    isl::io::writeToFile(
        std::filesystem::path("/Volumes/ramdisk").append("gll-final.dot"), tree_repr);
}

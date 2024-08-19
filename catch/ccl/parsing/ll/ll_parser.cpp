#include "ll_parser_for_math.hpp"
#include <ccl/parser/ll/ll_1_parser.hpp>

TEST_CASE("LLParser", "[LLParser]")
{
    using namespace ccl::debug;
    using namespace ccl::parser;

    reader::RulesReader math_rules_reader{SimpleLlGrammarDefinition, __FILE__};
    auto &math_rules_constructor = math_rules_reader.getParserBuilder();
    const auto &lexer = math_rules_constructor.getLexicalAnalyzer();
    auto parser = math_rules_constructor.buildLl1();

    auto tokenizer = lexer.getTokenizer("1 + 2 + 3 + 4");
    parser.parse(tokenizer);
}
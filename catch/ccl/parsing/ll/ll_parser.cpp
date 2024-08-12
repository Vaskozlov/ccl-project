#include "ccl/debug/debug.hpp"
#include "ccl/parser/ast/node_sequence.hpp"
#include "ccl/parser/ast/token_node.hpp"
#include "ll_parser_for_math.hpp"
#include <ccl/parser/ll/ll_1_parser.hpp>

TEST_CASE("LLParser", "[TreeParsing]")
{
    using namespace ccl::debug;
    using namespace ccl::parser;

    reader::RulesReader math_rules_reader{SimpleLlGrammarDefinition, __FILE__};
    auto &math_rules_constructor = math_rules_reader.getRulesConstructor(reader::Mode::LL);
    const auto math_rules_start_item = math_rules_constructor.getStartItem();
    auto &math_grammar_rules_storage = math_rules_constructor.getGrammarRulesStorage();

    auto &math_lexical_analyzer = math_rules_constructor.getLexicalAnalyzer();
    auto math_token_to_string = math_rules_constructor.getIdToNameTranslationFunction();

    for (auto &[key, rule] : math_grammar_rules_storage.rulesIterator()) {
        fmt::println(
            "{}: {}", math_token_to_string(key), std::views::transform(rule, math_token_to_string));
    }

    auto start_production = math_rules_constructor.getRuleId("GOAL");
    auto parser = ll::LL1{start_production, math_grammar_rules_storage, math_token_to_string};

    auto tokenizer = math_lexical_analyzer.getTokenizer("1+2");
    parser.parse(start_production, tokenizer)->print("", false, math_token_to_string);
}
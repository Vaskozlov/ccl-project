#include <ast-lang-2/ast-lang.hpp>
#include <benchmark/benchmark.h>
#include <ccl/parser/lr/lr_parser.hpp>
#include <ccl/parser/rules_reader/rules_lexer.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>

static void lrParserWithConstruction(benchmark::State &state)
{
    const auto *start_rule =
        ccl::parser::reader::getRulesGrammar().getStartRule(ccl::parser::reader::GOAL);

    const auto start_item = ccl::parser::LrItem{
        .dottedRule = {.rule = start_rule, .dotPosition = 0},
        .production = ccl::parser::reader::GOAL,
        .lookAhead = ccl::parser::reader::EOI,
    };

    for (auto _ : state) {
        auto tokenizer = ccl::parser::reader::RulesLexer.getTokenizer(astlang2::getAstlangGrammar());

        const auto id_to_str = ccl::lexer::lexerEnumToString<ccl::parser::reader::RulesLexerToken>;

        const auto lr_parser = ccl::parser::LrParser{
            start_item,
            ccl::parser::reader::EPSILON,
            ccl::parser::reader::getRulesGrammar(),
            id_to_str,
        };

        auto [root, algorithm] = lr_parser.parse(tokenizer);

        benchmark::DoNotOptimize(root);
    }
}

BENCHMARK(lrParserWithConstruction);

static void lrParserWithoutConstruction(benchmark::State &state)
{
    const auto *start_rule =
        ccl::parser::reader::getRulesGrammar().getStartRule(ccl::parser::reader::GOAL);

    const auto start_item = ccl::parser::LrItem{
        .dottedRule = {.rule = start_rule, .dotPosition = 0},
        .production = ccl::parser::reader::GOAL,
        .lookAhead = ccl::parser::reader::EOI,
    };

    const auto id_to_str = ccl::lexer::lexerEnumToString<ccl::parser::reader::RulesLexerToken>;

    const auto lr_parser = ccl::parser::LrParser{
        start_item,
        ccl::parser::reader::EPSILON,
        ccl::parser::reader::getRulesGrammar(),
        id_to_str,
    };

    for (auto _ : state) {
        auto tokenizer = ccl::parser::reader::RulesLexer.getTokenizer(astlang2::getAstlangGrammar());

        auto [root, algorithm] = lr_parser.parse(tokenizer);

        benchmark::DoNotOptimize(root);
    }
}

BENCHMARK(lrParserWithoutConstruction);

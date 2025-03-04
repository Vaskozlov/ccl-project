#include <ast-lang-2/ast-lang.hpp>
#include <benchmark/benchmark.h>
#include <ccl/parser/ll/gll_parser.hpp>
#include <ccl/parser/rules_reader/rules_lexer.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>

static void gllParserWithConstruction(benchmark::State &state)
{
    for (auto _ : state) {
        auto tokenizer =
            ccl::parser::reader::getRulesLexer().getTokenizer(astlang2::getAstlangGrammar());

        const auto id_to_str = ccl::lexer::lexerEnumToString<ccl::parser::reader::RulesLexerToken>;

        const auto gll_parser = ccl::parser::GllParser{
            ccl::parser::reader::GOAL,
            ccl::parser::reader::getRulesGrammar(),
            id_to_str,
        };

        auto [roots, algorithm] = gll_parser.parse(tokenizer);

        benchmark::DoNotOptimize(roots);
    }
}

BENCHMARK(gllParserWithConstruction);

static void gllParserWithoutConstruction(benchmark::State &state)
{
    const auto id_to_str = ccl::lexer::lexerEnumToString<ccl::parser::reader::RulesLexerToken>;

    const auto gll_parser = ccl::parser::GllParser{
        ccl::parser::reader::GOAL,
        ccl::parser::reader::getRulesGrammar(),
        id_to_str,
    };

    for (auto _ : state) {
        auto tokenizer =
            ccl::parser::reader::getRulesLexer().getTokenizer(astlang2::getAstlangGrammar());

        auto [roots, algorithm] = gll_parser.parse(tokenizer);

        benchmark::DoNotOptimize(roots);
    }
}

BENCHMARK(gllParserWithoutConstruction);

BENCHMARK_MAIN();

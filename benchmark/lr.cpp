#include <ast-lang/ast-lang.hpp>
#include <benchmark/benchmark.h>
#include <ccl/parser/lr/lr_parser.hpp>
#include <ccl/parser/rules_reader/rules_lexer.hpp>
#include <ccl/parser/rules_reader/rules_reader.hpp>

static void lrParserWithConstruction(benchmark::State &state)
{
    const auto *start_rule =
        ccl::parser::reader::RulesGrammar.getStartRule(ccl::parser::reader::GOAL);

    const auto start_item = ccl::parser::LrItem{
        .dottedRule = {.rule = start_rule, .dotPosition = 0},
        .production = ccl::parser::reader::GOAL,
        .lookAhead = ccl::parser::reader::EOI,
    };

    for (auto _ : state) {
        auto tokenizer = ccl::parser::reader::RulesLexer.getTokenizer(astlang::AstLangGrammar);

        const auto id_to_str = ccl::lexer::lexerEnumToString<ccl::parser::reader::RulesLexerToken>;

        const auto lr_parser = ccl::parser::LrParser{
            start_item,
            ccl::parser::reader::EPSILON,
            ccl::parser::reader::RulesGrammar,
            id_to_str,
        };

        const auto [root, algorithm] = lr_parser.parse(tokenizer);

        benchmark::DoNotOptimize(root);
    }
}

BENCHMARK(lrParserWithConstruction);

static void lrParserWithoutConstruction(benchmark::State &state)
{
    const auto *start_rule =
        ccl::parser::reader::RulesGrammar.getStartRule(ccl::parser::reader::GOAL);

    const auto start_item = ccl::parser::LrItem{
        .dottedRule = {.rule = start_rule, .dotPosition = 0},
        .production = ccl::parser::reader::GOAL,
        .lookAhead = ccl::parser::reader::EOI,
    };

    const auto id_to_str = ccl::lexer::lexerEnumToString<ccl::parser::reader::RulesLexerToken>;

    const auto lr_parser = ccl::parser::LrParser{
        start_item,
        ccl::parser::reader::EPSILON,
        ccl::parser::reader::RulesGrammar,
        id_to_str,
    };

    for (auto _ : state) {
        auto tokenizer = ccl::parser::reader::RulesLexer.getTokenizer(astlang::AstLangGrammar);

        const auto [root, algorithm] = lr_parser.parse(tokenizer);

        benchmark::DoNotOptimize(root);
    }
}

BENCHMARK(lrParserWithoutConstruction);

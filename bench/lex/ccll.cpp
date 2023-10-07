#include <benchmark/benchmark.h>
#include <ccl/debug/c_rule.hpp>
#include <ccl/lex/analyzer_generator/static_generator.hpp>

static auto ccllParsing(benchmark::State &state)
{
    for (auto _ : state) {
        ccl::lexer::AnalyzerGenerator::generateStaticVersion("builtin", ccl::debug::C_RULE);
    }
}

BENCHMARK(ccllParsing);
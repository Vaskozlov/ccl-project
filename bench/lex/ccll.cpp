#include <benchmark/benchmark.h>
import ccl;

static auto ccllParsing(benchmark::State &state)
{
    for (auto _ : state) {
        ccl::lexer::AnalyzerGenerator::generateStaticVersion("builtin", ccl::debug::C_RULE);
    }
}

BENCHMARK(ccllParsing);
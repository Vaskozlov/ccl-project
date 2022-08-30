#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>

using namespace ccl::string_view_literals;

auto C_RULE = R"(
CLASS_NAME = Test

[ARITHMETIC_OPERATOR]
ADD: '+'
SUB: '-
MUL '*'

)"_sv;

BOOST_AUTO_TEST_CASE(StaticGeneration)
{
    ccl::lex::AnalyzerGenerator::generateStaticVersion("C.rule", C_RULE);
}

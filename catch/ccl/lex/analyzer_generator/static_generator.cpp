#include <ccl/debug/c_rule.hpp>
#include <ccl/debug/debug.hpp>
#include <ccl/lexer/analyzer_generator/analyzer_generator.hpp>

TEST_CASE("StaticGeneration", "[StaticGenerator]")
{
    ccl::lexer::AnalyzerGenerator::generateStaticVersion(__FILE__, ccl::debug::C_RULE);
}

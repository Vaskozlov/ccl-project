#include <ccl/debug/debug_file.hpp>
import ccl.lexer.analyzer_generator;
import ccl.debug;

TEST_CASE("StaticGeneration", "[StaticGenerator]")
{
    ccl::lexer::generator::AnalyzerGenerator::generateStaticVersion(__FILE__, ccl::debug::C_RULE);
}

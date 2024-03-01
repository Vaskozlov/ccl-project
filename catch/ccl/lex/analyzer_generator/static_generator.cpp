#include <ccl/debug/debug_file.hpp>
import ccl.lexer.generator;
import ccl.debug;

TEST_CASE("StaticGeneration", "[StaticGenerator]")
{
    ccl::lexer::AnalyzerGenerator::generateStaticVersion(__FILE__, ccl::debug::C_RULE);
}

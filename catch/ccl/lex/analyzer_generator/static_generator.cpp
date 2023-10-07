#include <ccl/debug/c_rule.hpp>
#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>

using namespace ccl::string_view_literals;


TEST_CASE("StaticGeneration", "[StaticGenerator]")
{
    ccl::lexer::AnalyzerGenerator::generateStaticVersion(__FILE__, ccl::debug::C_RULE);
}

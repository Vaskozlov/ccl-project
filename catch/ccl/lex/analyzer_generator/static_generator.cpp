#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>

BOOST_AUTO_TEST_CASE(StaticGeneration)
{
    ccl::lex::AnalyzerGenerator::generateStaticVersion(
        "/home/vaskozlov/projects/ccl-project/catch/ccl/lex/analyzer_generator/C.rule");
}

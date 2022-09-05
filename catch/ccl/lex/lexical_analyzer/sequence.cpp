#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/lexical_analyzer.hpp>

using namespace ccl;
using namespace lex;
using namespace text;

BOOST_AUTO_TEST_CASE(LexicalAnalyzerSequence, *utf::depends_on("ContainerSequence"))
{
    auto analyzer = LexicalAnalyzer(
        ExceptionHandler::instance(), { { 2, R"("abz"p"10")" }, { 3, R"("10""abz"p)" } });

    auto tokenizer = analyzer.getTokenizer(R"(abz10 10abz)");
    auto token = tokenizer.yield();

}

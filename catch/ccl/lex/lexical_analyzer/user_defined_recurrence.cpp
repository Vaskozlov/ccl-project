#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/lexical_analyzer.hpp>

using namespace ccl;
using namespace lex;
using namespace text;

BOOST_AUTO_TEST_CASE(
    UserDefinedRecurrence,
    *utf::depends_on("ContainerUnion") * utf::depends_on("ContainerRecurrence"))
{
    auto analyzer = LexicalAnalyzer{
        ExceptionHandler::instance(),
        { { 1, R"([a-z]{1, 3}[a-z])" } },
    };
    auto tokenizer = analyzer.getTokenizer(R"(abcd)");

    DEBUG_VAR token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == "abcd");
}

#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/lexical_analyzer.hpp>

using namespace ccl;
using namespace lex;
using namespace text;

BOOST_AUTO_TEST_CASE(LexicalAnalyzerUnion, *utf::depends_on("ContainerUnion"))
{
    auto analyzer = LexicalAnalyzer{
        ExceptionHandler::instance(),
        { { 1, R"([a-z]p+[0-9]+)" }, { 2, R"([0-9]+[a-z]p*)" } },
    };

    auto tokenizer = analyzer.getTokenizer(R"(20 abz10 10abz)");

    auto token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == 2);
    BOOST_ASSERT(token.getRepr() == "20");
    BOOST_ASSERT(token.getValue() == "20");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().empty());

    token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == R"(abz10)");
    BOOST_ASSERT(token.getValue() == "abz10");
    BOOST_ASSERT(token.getPrefixes().size() == 1);
    BOOST_ASSERT(token.getPostfixes().empty());
    BOOST_ASSERT(token.getPrefixes()[0] == "abz");

    token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == 2);
    BOOST_ASSERT(token.getRepr() == R"(10abz)");
    BOOST_ASSERT(token.getValue() == "10abz");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().size() == 1);
    BOOST_ASSERT(token.getPostfixes()[0] == "abz");
}

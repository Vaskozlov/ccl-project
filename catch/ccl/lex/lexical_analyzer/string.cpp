#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/lexical_analyzer.hpp>

using namespace ccl;
using namespace lex;
using namespace text;

BOOST_AUTO_TEST_SUITE(
    LexicalAnalyzerString, *utf::depends_on("ContainerStringsAndCharactersCreation"))

BOOST_AUTO_TEST_CASE(LexicalAnalyzerBasicString)
{
    auto analyzer = LexicalAnalyzer{ ExceptionHandler::instance(), { { 2, R"("\""s)" } } };
    auto tokenizer = analyzer.getTokenizer(R"("some string")");
    auto token = tokenizer.yield();

    BOOST_ASSERT(token.getRepr() == R"("some string")");
    BOOST_ASSERT(token.getValue() == "some string");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().empty());
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerRawString)
{
    auto analyzer = LexicalAnalyzer{ ExceptionHandler::instance(), { { 2, R"( "R\"(:)\""m )" } } };
    auto tokenizer = analyzer.getTokenizer("R\"(some\nstring)\"");
    auto token = tokenizer.yield();

    BOOST_ASSERT(token.getRepr() == "R\"(some\nstring)\"");
    BOOST_ASSERT(token.getValue() == "some\nstring");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().empty());
}

BOOST_AUTO_TEST_SUITE_END()

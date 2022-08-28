#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/lexical_analyzer.hpp>

using namespace ccl;
using namespace lex;
using namespace text;

BOOST_AUTO_TEST_SUITE(
    LexicalAnalyzerString, *utf::depends_on("ContainerStringsAndCharactersCreation"))

BOOST_AUTO_TEST_CASE(LexicalAnalyzerBasicString)
{
    auto analyzer = LexicalAnalyzer{ { 1, u8R"("\""s)" } };
    auto tokenizer = analyzer.getTokenizer(u8R"("some string")");
    auto token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == u8R"("some string")");
    BOOST_ASSERT(token.getValue() == u8"some string");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().empty());
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerRawString)
{
    auto analyzer = LexicalAnalyzer{ { 1, u8R"( "R\"(:)\""m )" } };
    auto tokenizer = analyzer.getTokenizer(u8"R\"(some\nstring)\"");
    auto token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == u8"R\"(some\nstring)\"");
    BOOST_ASSERT(token.getValue() == u8"some\nstring");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().empty());
}

BOOST_AUTO_TEST_SUITE_END()

#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

using namespace cerb::lex;
using namespace cerb::text;

BOOST_AUTO_TEST_SUITE(
    LexicalAnalyzerString, *utf::depends_on("DotItemStringsAndCharactersCreation"))

BOOST_AUTO_TEST_CASE(LexicalAnalyzerBasicString)
{
    auto text = TextIterator{ u8R"("some string")" };
    auto analyzer = LexicalAnalyzer{ { 1, u8R"("\""s)" } };
    auto token = analyzer.yield(text);

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == u8R"("some string")");
    BOOST_ASSERT(token.getValue() == u8"some string");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().empty());
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerRawString)
{
    auto text = TextIterator{ u8"R\"(some\nstring)\"" };
    auto analyzer = LexicalAnalyzer{ { 1, u8R"( "R\"(:)\""m )" } };
    auto token = analyzer.yield(text);

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == u8"R\"(some\nstring)\"");
    BOOST_ASSERT(token.getValue() == u8"some\nstring");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().empty());
}

BOOST_AUTO_TEST_SUITE_END()

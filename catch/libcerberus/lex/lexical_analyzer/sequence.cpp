#include <boost/test/unit_test.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

using namespace cerb::lex;
using namespace cerb::text;

BOOST_AUTO_TEST_CASE(LexicalAnalyzerSequence)
{
    auto text = TextIterator{ u8R"(abz10 10abz)" };
    auto analyzer = LexicalAnalyzer{ { 1, u8R"("abz"p"10")" }, { 2, u8R"("10""abz"p)" } };
    auto token = analyzer.yield(text);

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == u8R"(abz10)");
    BOOST_ASSERT(token.getValue() == u8"abz10");
    BOOST_ASSERT(token.getPrefixes().size() == 1);
    BOOST_ASSERT(token.getPrefixes()[0] == u8"abz");

    token = analyzer.yield(text);

    BOOST_ASSERT(token.getId() == 2);
    BOOST_ASSERT(token.getRepr() == u8R"(10abz)");
    BOOST_ASSERT(token.getValue() == u8"10abz");
    BOOST_ASSERT(token.getPostfixes().size() == 1);
    BOOST_ASSERT(token.getPostfixes()[0] == u8"abz");
}

#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

using namespace cerb::lex;
using namespace cerb::text;

BOOST_AUTO_TEST_CASE(LexicalAnalyzerUnion, *utf::depends_on("DotItemUnion"))
{
    auto text = TextIterator{ u8R"(20 abz10 10abz)" };
    auto analyzer = LexicalAnalyzer{
        { 1, u8R"([a-z]p*[0-9]+)" },
        { 2, u8R"([0-9]+[a-z]p*)" },
    };

    auto token = analyzer.yield(text);

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == u8"20");
    BOOST_ASSERT(token.getValue() == u8"20");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().empty());

    token = analyzer.yield(text);

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == u8R"(abz10)");
    BOOST_ASSERT(token.getValue() == u8"abz10");
    BOOST_ASSERT(token.getPrefixes().size() == 1);
    BOOST_ASSERT(token.getPostfixes().empty());
    BOOST_ASSERT(token.getPrefixes()[0] == u8"abz");

    token = analyzer.yield(text);

    BOOST_ASSERT(token.getId() == 2);
    BOOST_ASSERT(token.getRepr() == u8R"(10abz)");
    BOOST_ASSERT(token.getValue() == u8"10abz");
    BOOST_ASSERT(token.getPrefixes().empty());
    BOOST_ASSERT(token.getPostfixes().size() == 1);
    BOOST_ASSERT(token.getPostfixes()[0] == u8"abz");
}

#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/lexical_analyzer.hpp>

using namespace ccl;
using namespace lex;
using namespace text;

BOOST_AUTO_TEST_CASE(LexicalAnalyzerSequence, *utf::depends_on("ContainerSequence"))
{
    auto analyzer = LexicalAnalyzer{ { 1, u8R"("abz"p"10")" }, { 2, u8R"("10""abz"p)" } };
    auto tokenizer = analyzer.getTokenizer(u8R"(abz10 10abz)");

    auto token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == u8R"(abz10)");
    BOOST_ASSERT(token.getValue() == u8"abz10");
    BOOST_ASSERT(token.getPrefixes().size() == 1);
    BOOST_ASSERT(token.getPrefixes()[0] == u8"abz");

    token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == 2);
    BOOST_ASSERT(token.getRepr() == u8R"(10abz)");
    BOOST_ASSERT(token.getValue() == u8"10abz");
    BOOST_ASSERT(token.getPostfixes().size() == 1);
    BOOST_ASSERT(token.getPostfixes()[0] == u8"abz");
}

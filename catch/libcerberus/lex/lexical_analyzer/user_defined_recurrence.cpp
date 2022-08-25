#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

using namespace cerb::lex;
using namespace cerb::text;

BOOST_AUTO_TEST_CASE(
    UserDefinedRecurrence, *utf::depends_on("DotItemUnion") * utf::depends_on("DotItemRecurrence"))
{
    auto analyzer = LexicalAnalyzer{
        { 1, u8R"([a-z]{1, 3}[a-z])" },
    };
    auto tokenizer = analyzer.getTokenizer(u8R"(abcd)");

    DEBUG_DECL token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == 1);
    BOOST_ASSERT(token.getRepr() == u8"abcd");
}

#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

using namespace cerb::lex;
using namespace cerb::text;

BOOST_AUTO_TEST_CASE(LexicalAnalyzerUnrecognizableTokenError)
{
    auto analyzer = LexicalAnalyzer{ { 1, u8"[a-z]+" } };
    auto tokenizer = analyzer.getTokenizer(u8R"(20)");
    auto token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == ReservedTokenType::BAD_TOKEN);
    BOOST_ASSERT(
        analyzer.getExceptionAccumulator().getErrors()[0]->getMessage() ==
        u8"unrecognizable token");
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerUnrecognizableToken)
{
    auto analyzer = LexicalAnalyzer{ { 1, u8"[a-z]+" } };
    auto tokenizer = analyzer.getTokenizer(u8R"(20)");

    DEBUG_DECL bad_token = tokenizer.yield();

    BOOST_ASSERT(bad_token.getId() == ReservedTokenType::BAD_TOKEN);
    BOOST_ASSERT(bad_token.getRepr() == u8"20");
}

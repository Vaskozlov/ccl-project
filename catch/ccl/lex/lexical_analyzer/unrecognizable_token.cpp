#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/lexical_analyzer.hpp>

using namespace ccl;
using namespace lex;
using namespace text;

BOOST_AUTO_TEST_CASE(LexicalAnalyzerUnrecognizableTokenError)
{
    auto analyzer = LexicalAnalyzer{ { 1, u8"[a-z]+" } };
    auto tokenizer = analyzer.getTokenizer(u8R"(20)");
    auto token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == ReservedTokenType::BAD_TOKEN);
    BOOST_ASSERT(tokenizer.getErrors()[0]->getMessage() == u8"unrecognizable token");
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerUnrecognizableToken)
{
    auto analyzer = LexicalAnalyzer{ { 1, u8"[a-z]+" } };
    auto tokenizer = analyzer.getTokenizer(u8R"(20)");

    DEBUG_VAR bad_token = tokenizer.yield();

    BOOST_ASSERT(bad_token.getId() == ReservedTokenType::BAD_TOKEN);
    BOOST_ASSERT(bad_token.getRepr() == u8"20");
}

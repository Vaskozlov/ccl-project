#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/lexical_analyzer.hpp>

using namespace ccl;
using namespace lex;
using namespace text;

BOOST_AUTO_TEST_CASE(LexicalAnalyzerUnrecognizableTokenError)
{
    auto analyzer = LexicalAnalyzer{ ExceptionHandler::instance(), { { 2, "[a-z]+" } } };
    auto tokenizer = analyzer.getTokenizer(R"(20)");
    auto token = tokenizer.yield();

    BOOST_ASSERT(token.getId() == ReservedTokenType::BAD_TOKEN);
}

BOOST_AUTO_TEST_CASE(LexicalAnalyzerUnrecognizableToken)
{
    auto analyzer = LexicalAnalyzer{ ExceptionHandler::instance(), { { 2, "[a-z]+" } } };
    auto tokenizer = analyzer.getTokenizer(R"(20)");

    DEBUG_VAR bad_token = tokenizer.yield();

    BOOST_ASSERT(bad_token.getId() == ReservedTokenType::BAD_TOKEN);
    BOOST_ASSERT(bad_token.getRepr() == "20");
}
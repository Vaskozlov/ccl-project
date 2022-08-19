#include <boost/test/unit_test.hpp>
#include <cerberus/lex/lexical_analyzer.hpp>

using namespace cerb::lex;
using namespace cerb::text;

BOOST_AUTO_TEST_CASE(LexicalAnalyzerUnrecognizableToken)
{
    auto text = TextIterator{ u8R"(20)" };
    auto analyzer = LexicalAnalyzer{ { 1, u8"[a-z]+" } };

    BOOST_CHECK_EXCEPTION(
        analyzer.yield(text), LexicalAnalysisException, [](const LexicalAnalysisException &e) {
            return e.getMessage() == u8"unrecognizable token";
        });
}

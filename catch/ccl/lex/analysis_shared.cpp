#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/analysis_shared.hpp>

using namespace ccl;
using namespace lex;
using namespace text;

BOOST_AUTO_TEST_SUITE(LexAnalysisShared)

BOOST_AUTO_TEST_CASE(AnalysisSharedTestComments)
{
    auto text_iterator = TextIterator{ "/* test */" };
    auto analysis_shared = AnalysisShared{ { "//", "/*", "*/" } };

    BOOST_ASSERT(analysis_shared.isComment(text_iterator.getFutureRemaining(1)));
    BOOST_ASSERT(analysis_shared.isNextCharNotForScanning(text_iterator));
}

BOOST_AUTO_TEST_CASE(AnalysisSharedTestTerminals)
{
    auto text_iterator = TextIterator{ "+" };
    auto analysis_shared = AnalysisShared{ {}, {}, { { "+", 1 } } };

    BOOST_ASSERT(analysis_shared.isTerminal(text_iterator.getFutureRemaining(1)));
    BOOST_ASSERT(analysis_shared.isNextCharNotForScanning(text_iterator));
}

BOOST_AUTO_TEST_CASE(AnalysisSharedTestStrings)
{
    auto text_iterator = TextIterator{ R"("")" };
    auto analysis_shared = AnalysisShared{ {}, { { "\"", 0, false, false } } };

    BOOST_ASSERT(analysis_shared.isNextCharNotForScanning(text_iterator));
}

BOOST_AUTO_TEST_SUITE_END()

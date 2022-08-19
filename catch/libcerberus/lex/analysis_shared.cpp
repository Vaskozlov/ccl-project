#include <boost/test/unit_test.hpp>
#include <cerberus/lex/analysis_shared.hpp>

using namespace cerb::lex;
using namespace cerb::text;

BOOST_AUTO_TEST_SUITE(LexAnalysisShared)

BOOST_AUTO_TEST_CASE(AnalysisSharedTestComments)
{
    auto text_iterator = TextIterator{ u8"/* test */" };
    auto analysis_shared = AnalysisShared{ { u8"//", u8"/*", u8"*/" } };

    BOOST_ASSERT(analysis_shared.isComment(text_iterator.getFutureRemaining(1)));
    BOOST_ASSERT(analysis_shared.isNextCharNotForScanning(text_iterator));
}

BOOST_AUTO_TEST_CASE(AnalysisSharedTestTerminals)
{
    auto text_iterator = TextIterator{ u8"+" };
    auto analysis_shared = AnalysisShared{ {}, {}, { { u8"+", 1 } } };

    BOOST_ASSERT(analysis_shared.isTerminal(text_iterator.getFutureRemaining(1)));
    BOOST_ASSERT(analysis_shared.isNextCharNotForScanning(text_iterator));
}

BOOST_AUTO_TEST_CASE(AnalysisSharedTestStrings)
{
    auto text_iterator = TextIterator{ u8R"("")" };
    auto analysis_shared = AnalysisShared{ {}, { { u8"\"", 0, false, false } } };

    BOOST_ASSERT(analysis_shared.isNextCharNotForScanning(text_iterator));
}

BOOST_AUTO_TEST_SUITE_END()

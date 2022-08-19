#include <boost/test/unit_test.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;

BOOST_AUTO_TEST_SUITE(TextIteratorSingleLine)

BOOST_AUTO_TEST_CASE(TextIteratorUtfSingleLineCommentSkipping)
{
    auto text_iterator = TextIterator{ u8"\U0001F600\nHi!", nullptr, { u8"\U0001F600" } };
    text_iterator.skipCommentsAndLayout();

    BOOST_ASSERT(text_iterator.nextRawChar() == U'H');
}

BOOST_AUTO_TEST_CASE(TextIteratorSingleLineCommentWithSkippingForwardingEoF)
{
    auto text_iterator = TextIterator{ u8"Hi! //", nullptr, { u8"//" } };
    text_iterator.rawSymbolsSkip(3);
    text_iterator.skipCommentsAndLayout();

    BOOST_ASSERT(text_iterator.nextRawChar() == U'\0');
}

BOOST_AUTO_TEST_CASE(TextIteratorSingleLineCommentWithSkippingForwardingNewLine)
{
    auto text_iterator = TextIterator{ u8"Hi! //\nHello!", nullptr, { u8"//" } };
    text_iterator.rawSymbolsSkip(3);
    text_iterator.skipCommentsAndLayout();

    BOOST_ASSERT(text_iterator.nextRawChar() == U'H');
}

BOOST_AUTO_TEST_CASE(TextIteratorTwoSingleLineCommentWithSkippingF)
{
    auto text_iterator = TextIterator{ u8"Hi! //\n //\nHello!", nullptr, { u8"//" } };
    text_iterator.rawSymbolsSkip(3);
    text_iterator.skipCommentsAndLayout();

    BOOST_ASSERT(text_iterator.nextRawChar() == U'H');
}

BOOST_AUTO_TEST_SUITE_END()

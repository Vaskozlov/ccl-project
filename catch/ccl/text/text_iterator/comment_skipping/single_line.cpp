#include <ccl/debug/debug_file.hpp>
#include <ccl/text/text_iterator.hpp>

using namespace ccl;
using namespace text;

BOOST_AUTO_TEST_SUITE(TextIteratorSingleLine)

BOOST_AUTO_TEST_CASE(TextIteratorUtfSingleLineCommentSkipping)
{
    auto text_iterator =
        TextIterator{ "\U0001F600\nHi!", ExceptionHandler::instance(), { "\U0001F600" } };
    text_iterator.skipCommentsAndLayout();

    BOOST_ASSERT(text_iterator.next() == U'H');
}

BOOST_AUTO_TEST_CASE(TextIteratorSingleLineCommentWithSkippingForwardingEoF)
{
    auto text_iterator = TextIterator{ "Hi! //", ExceptionHandler::instance(), { "//" } };
    text_iterator.skipCharacters(3);
    text_iterator.skipCommentsAndLayout();

    BOOST_ASSERT(text_iterator.next() == U'\0');
}

BOOST_AUTO_TEST_CASE(TextIteratorSingleLineCommentWithSkippingForwardingNewLine)
{
    auto text_iterator = TextIterator{ "Hi! //\nHello!", ExceptionHandler::instance(), { "//" } };
    text_iterator.skipCharacters(3);
    text_iterator.skipCommentsAndLayout();

    BOOST_ASSERT(text_iterator.next() == U'H');
}

BOOST_AUTO_TEST_CASE(TextIteratorTwoSingleLineCommentWithSkippingF)
{
    auto text_iterator =
        TextIterator{ "Hi! //\n //\nHello!", ExceptionHandler::instance(), { "//" } };
    text_iterator.skipCharacters(3);
    text_iterator.skipCommentsAndLayout();

    BOOST_ASSERT(text_iterator.next() == U'H');
}

BOOST_AUTO_TEST_SUITE_END()

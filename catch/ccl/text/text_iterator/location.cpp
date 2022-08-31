#include <ccl/debug/debug_file.hpp>
#include <ccl/text/text_iterator.hpp>

using namespace ccl;
using namespace text;
using namespace string_view_literals;

BOOST_AUTO_TEST_CASE(TextIteratorLocationTrackingOnEmptyInput)
{
    auto text_iterator = TextIterator{ ""_sv };

    BOOST_ASSERT(text_iterator.getLine() == 1);
    BOOST_ASSERT(text_iterator.getColumn() == 0);

    text_iterator.next();

    BOOST_ASSERT(text_iterator.getLine() == 1);
    BOOST_ASSERT(text_iterator.getColumn() == 0);

    text_iterator.next();

    BOOST_ASSERT(text_iterator.getLine() == 1);
    BOOST_ASSERT(text_iterator.getColumn() == 0);
}

BOOST_AUTO_TEST_CASE(TextIteratorLocationTrackingWithUtfSymbols)
{
    auto text_iterator = TextIterator{ "Hi\n\U0001f000World!"_sv };

    BOOST_ASSERT(text_iterator.getLine() == 1);
    BOOST_ASSERT(text_iterator.getColumn() == 0);
    BOOST_ASSERT(text_iterator.getRealColumn() == 0);

    BOOST_ASSERT(text_iterator.next() == U'H');

    BOOST_ASSERT(text_iterator.getLine() == 1);
    BOOST_ASSERT(text_iterator.getColumn() == 1);
    BOOST_ASSERT(text_iterator.getRealColumn() == 1);

    BOOST_ASSERT(text_iterator.next() == U'i');

    BOOST_ASSERT(text_iterator.getLine() == 1);
    BOOST_ASSERT(text_iterator.getColumn() == 2);
    BOOST_ASSERT(text_iterator.getRealColumn() == 2);

    BOOST_ASSERT(text_iterator.next() == U'\n');

    BOOST_ASSERT(text_iterator.getLine() == 2);
    BOOST_ASSERT(text_iterator.getColumn() == 0);
    BOOST_ASSERT(text_iterator.getRealColumn() == 0);

    BOOST_ASSERT(text_iterator.next() == U'\U0001f000');

    BOOST_ASSERT(text_iterator.getLine() == 2);
    BOOST_ASSERT(text_iterator.getColumn() == 1);
    BOOST_ASSERT(text_iterator.getRealColumn() == 4);

    BOOST_ASSERT(text_iterator.next() == U'W');

    BOOST_ASSERT(text_iterator.getLine() == 2);
    BOOST_ASSERT(text_iterator.getColumn() == 2);
    BOOST_ASSERT(text_iterator.getRealColumn() == 5);// NOLINT
}
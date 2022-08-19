#include <boost/test/unit_test.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::string_view_literals;

BOOST_AUTO_TEST_CASE(TextIteratorTabsAndSpaceTracking)
{
    auto text_iterator = TextIterator{ u8"\t\t  H i\nWorld!"_sv };

    text_iterator.moveToCleanChar();
    text_iterator.nextRawChar();

    BOOST_ASSERT(text_iterator.getTabsAndSpaces() == U"\t\t  ");

    text_iterator.moveToCleanChar();
    text_iterator.nextRawChar();

    BOOST_ASSERT(text_iterator.getTabsAndSpaces() == U" ");
}

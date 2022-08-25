#include <ccl/debug/debug_file.hpp>
#include <ccl/text/text_iterator.hpp>

using namespace ccl;
using namespace text;
using namespace string_view_literals;

BOOST_AUTO_TEST_CASE(TextIteratorTabsAndSpaceTracking)
{
    auto text_iterator = TextIterator{ u8"\t\t  H i\nWorld!"_sv };

    text_iterator.moveToCleanChar();
    text_iterator.next();

    BOOST_ASSERT(text_iterator.getTabsAndSpaces() == U"\t\t  ");

    text_iterator.moveToCleanChar();
    text_iterator.next();

    BOOST_ASSERT(text_iterator.getTabsAndSpaces() == U" ");
}

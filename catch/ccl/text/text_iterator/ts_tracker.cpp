#include <ccl/debug/debug_file.hpp>
#include <ccl/text/text_iterator.hpp>

using namespace ccl;
using namespace text;
using namespace string_view_literals;

TEST_CASE("TextIteratorTabsAndSpaceTracking", "[TabsAndSpacesTracker]")
{
    auto text_iterator = TextIterator{"\t\t  H i\nWorld!"_sv};

    text_iterator.moveToCleanChar();
    text_iterator.advance();

    REQUIRE(text_iterator.getTabsAndSpaces() == "\t\t  ");

    text_iterator.moveToCleanChar();
    text_iterator.advance();

    REQUIRE(text_iterator.getTabsAndSpaces() == " ");
}

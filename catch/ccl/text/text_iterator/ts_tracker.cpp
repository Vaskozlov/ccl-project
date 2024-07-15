#include <ccl/debug/debug.hpp>
#include <ccl/text/text_iterator.hpp>

TEST_CASE("TextIteratorTabsAndSpaceTracking", "[TabsAndSpacesTracker]")
{
    using namespace ccl;
    using namespace isl::string_view_literals;

    auto text_iterator = text::TextIterator{"\t\t  H i\nWorld!"_sv};

    text_iterator.moveToCleanChar();
    text_iterator.advance();

    REQUIRE(text_iterator.getTabsAndSpaces() == "\t\t  ");

    text_iterator.moveToCleanChar();
    text_iterator.advance();

    REQUIRE(text_iterator.getTabsAndSpaces() == " ");
}

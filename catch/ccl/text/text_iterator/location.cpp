#include <ccl/debug/debug_file.hpp>
#include <ccl/text/text_iterator.hpp>

using namespace ccl;
using namespace text;
using namespace string_view_literals;

TEST_CASE("TextIteratorLocationTrackingOnEmptyInput", "[TextIterator]")
{
    auto text_iterator = TextIterator{""_sv};

    REQUIRE(text_iterator.getLine() == 1);
    REQUIRE(text_iterator.getColumn() == 0);

    text_iterator.next();

    REQUIRE(text_iterator.getLine() == 1);
    REQUIRE(text_iterator.getColumn() == 0);

    text_iterator.next();

    REQUIRE(text_iterator.getLine() == 1);
    REQUIRE(text_iterator.getColumn() == 0);
}

TEST_CASE("TextIteratorLocationTrackingWithUtfSymbols", "[TextIterator]")
{
    auto text_iterator = TextIterator{"Hi\n\U0001f000World!"_sv};

    REQUIRE(text_iterator.getLine() == 1);
    REQUIRE(text_iterator.getColumn() == 0);
    REQUIRE(text_iterator.getRealColumn() == 0);

    REQUIRE(text_iterator.next() == U'H');

    REQUIRE(text_iterator.getLine() == 1);
    REQUIRE(text_iterator.getColumn() == 1);
    REQUIRE(text_iterator.getRealColumn() == 1);

    REQUIRE(text_iterator.next() == U'i');

    REQUIRE(text_iterator.getLine() == 1);
    REQUIRE(text_iterator.getColumn() == 2);
    REQUIRE(text_iterator.getRealColumn() == 2);

    REQUIRE(text_iterator.next() == U'\n');

    REQUIRE(text_iterator.getLine() == 2);
    REQUIRE(text_iterator.getColumn() == 0);
    REQUIRE(text_iterator.getRealColumn() == 0);

    REQUIRE(text_iterator.next() == U'\U0001f000');

    REQUIRE(text_iterator.getLine() == 2);
    REQUIRE(text_iterator.getColumn() == 1);
    REQUIRE(text_iterator.getRealColumn() == 4);

    REQUIRE(text_iterator.next() == U'W');

    REQUIRE(text_iterator.getLine() == 2);
    REQUIRE(text_iterator.getColumn() == 2);
    REQUIRE(text_iterator.getRealColumn() == 5);// NOLINT
}

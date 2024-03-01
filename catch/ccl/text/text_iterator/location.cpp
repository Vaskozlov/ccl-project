#include <ccl/debug/debug_file.hpp>
import ccl.text;

TEST_CASE("TextIteratorLocationTrackingOnEmptyInput", "[TextIterator]")
{
    using namespace ccl;
    using namespace isl::string_view_literals;

    auto text_iterator = text::TextIterator{""_sv};

    REQUIRE(text_iterator.getLine() == 1);
    REQUIRE(text_iterator.getColumn() == 0);

    text_iterator.advance();

    REQUIRE(text_iterator.getLine() == 1);
    REQUIRE(text_iterator.getColumn() == 0);

    text_iterator.advance();

    REQUIRE(text_iterator.getLine() == 1);
    REQUIRE(text_iterator.getColumn() == 0);
}

TEST_CASE("TextIteratorLocationTrackingWithUtfSymbols", "[TextIterator]")
{
    using namespace ccl;
    using namespace isl::string_view_literals;

    auto text_iterator = text::TextIterator{"Hi\n\U0001f000World!"_sv};

    REQUIRE(text_iterator.getLine() == 1);
    REQUIRE(text_iterator.getColumn() == 0);
    REQUIRE(text_iterator.getRealColumn() == 0);

    REQUIRE(text_iterator.advance() == U'H');

    REQUIRE(text_iterator.getLine() == 1);
    REQUIRE(text_iterator.getColumn() == 1);
    REQUIRE(text_iterator.getRealColumn() == 1);

    REQUIRE(text_iterator.advance() == U'i');

    REQUIRE(text_iterator.getLine() == 1);
    REQUIRE(text_iterator.getColumn() == 2);
    REQUIRE(text_iterator.getRealColumn() == 2);

    REQUIRE(text_iterator.advance() == U'\n');

    REQUIRE(text_iterator.getLine() == 2);
    REQUIRE(text_iterator.getColumn() == 0);
    REQUIRE(text_iterator.getRealColumn() == 0);

    REQUIRE(text_iterator.advance() == U'\U0001f000');

    REQUIRE(text_iterator.getLine() == 2);
    REQUIRE(text_iterator.getColumn() == 1);
    REQUIRE(text_iterator.getRealColumn() == 4);

    REQUIRE(text_iterator.advance() == U'W');

    REQUIRE(text_iterator.getLine() == 2);
    REQUIRE(text_iterator.getColumn() == 2);
    REQUIRE(text_iterator.getRealColumn() == 5);// NOLINT
}

#include <ccl/debug/debug_file.hpp>
import ccl.text;

TEST_CASE("BasicTextIteratorRawIteration", "[TextIterator]")
{
    using namespace ccl;
    using namespace isl::string_view_literals;

    auto input = "Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;
    auto expected = U"Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;
    auto text_iterator = text::BasicTextIterator{input};

    for (DEBUG_VAR chr : expected) {
        DEBUG_VAR symbol = text_iterator.advance();

        REQUIRE(symbol == chr);
        REQUIRE(text_iterator.getCurrentChar() == chr);
    }
}

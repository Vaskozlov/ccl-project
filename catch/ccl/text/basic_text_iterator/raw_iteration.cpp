#include <ccl/debug/debug_file.hpp>
#include <ccl/text/basic_text_iterator.hpp>

using namespace ccl;
using namespace text;
using namespace isl::string_view_literals;

TEST_CASE("BasicTextIteratorRawIteration", "[TextIterator]")
{
    auto input = "Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;
    auto expected = U"Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;
    auto text_iterator = BasicTextIterator{input};

    for (DEBUG_VAR &&chr : expected) {
        DEBUG_VAR sym = text_iterator.advance();

        REQUIRE(sym == chr);
        REQUIRE(text_iterator.getCurrentChar() == chr);
    }
}

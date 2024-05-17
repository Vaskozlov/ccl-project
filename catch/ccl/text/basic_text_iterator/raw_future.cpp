#include <ccl/debug/debug_file.hpp>
#include <ccl/text/basic_text_iterator.hpp>

TEST_CASE("BasicTextIteratorRawFuture", "[TextIterator]")
{
    using namespace ccl;
    using namespace isl::string_view_literals;

    auto input = "Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;
    auto expected = U"Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;

    DEBUG_VAR index = isl::as<std::size_t>(1);
    DEBUG_VAR text_iterator = text::BasicTextIterator{input};

    for (DEBUG_VAR chr : expected) {
        REQUIRE(text_iterator.futureChar() == chr);
        ++index;
        text_iterator.advance();
    }
}

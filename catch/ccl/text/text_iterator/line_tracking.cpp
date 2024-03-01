#include <ccl/debug/debug_file.hpp>
import ccl.text;

TEST_CASE("TextIteratorLineTracking", "[TextIteratorLineTracker]")
{
    using namespace ccl;
    using namespace isl::string_view_literals;

    auto text_iterator = text::TextIterator{"Hi\nWorld!"_sv};

    for (auto i = isl::as<std::size_t>(0); i != 3; ++i) {
        text_iterator.advance();
        REQUIRE(text_iterator.getWorkingLine() == "Hi");
    }

    text_iterator.advance();
    REQUIRE(text_iterator.getWorkingLine() == "World!");
}

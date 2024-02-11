#include <ccl/debug/debug_file.hpp>
#include <ccl/text/text_iterator.hpp>

TEST_CASE("TextIteratorLineTracking", "[TextIteratorLineTracker]")
{
    using namespace ccl;
    using namespace isl::string_view_literals;

    auto text_iterator = text::TextIterator{"Hi\nWorld!"_sv};

    for (auto i = isl::as<size_t>(0); i != 3; ++i) {
        text_iterator.advance();
        REQUIRE(text_iterator.getWorkingLine() == "Hi");
    }

    text_iterator.advance();
    REQUIRE(text_iterator.getWorkingLine() == "World!");
}

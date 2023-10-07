#include <ccl/debug/debug_file.hpp>
#include <ccl/text/text_iterator.hpp>

using namespace ccl;
using namespace text;
using namespace string_view_literals;

TEST_CASE("TextIteratorLineTracking", "[TextIteratorLineTracker]")
{
    auto text_iterator = TextIterator{"Hi\nWorld!"_sv};

    for (auto i = as<size_t>(0); i != 3; ++i) {
        text_iterator.advance();
        REQUIRE(text_iterator.getWorkingLine() == "Hi");
    }

    text_iterator.advance();
    REQUIRE(text_iterator.getWorkingLine() == "World!");
}

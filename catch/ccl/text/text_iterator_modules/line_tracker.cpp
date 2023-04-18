#include <ccl/debug/debug_file.hpp>
#include <ccl/text/text_iterator_modules/line_tracker.hpp>

using namespace ccl;
using namespace text;
using namespace string_view_literals;

constexpr static auto Input = "Hello, World!\nIt's a \nTest!\nOf line tracker\n "_sv;

[[maybe_unused]] constexpr static auto ExpectedLines = std::array<string_view, 6>{
    "Hello, World!", "It's a ", "Test!",
    "Of line "
    "tracker",
    " "};

TEST_CASE("LineTracking", "[LineTracker]")
{
    DEBUG_VAR current_line = as<size_t>(0);
    DEBUG_VAR line_tracker = LineTracker{Input};

    for (auto chr : Input) {
        line_tracker.next(as<char32_t>(chr));

        if ('\n' == chr) {
            REQUIRE(ExpectedLines.at(current_line) == line_tracker.get());
            ++current_line;
        }
    }

    REQUIRE(ExpectedLines.at(current_line) == line_tracker.get());
}

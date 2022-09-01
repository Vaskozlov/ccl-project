#include <ccl/debug/debug_file.hpp>
#include <ccl/text/text_iterator_modules/line_tracker.hpp>

using namespace ccl;
using namespace text;
using namespace text::module;
using namespace string_view_literals;

constexpr static auto Input = "Hello, World!\nIt's a \nTest!\nOf line tracker\n "_sv;

[[maybe_unused]] constexpr static auto ExpectedLines =
    std::array{ "Hello, World!", "It's a ", "Test!",
                "Of line "
                "tracker",
                " " };

BOOST_AUTO_TEST_CASE(LineTrackig)
{
    DEBUG_VAR current_line = static_cast<size_t>(0);
    DEBUG_VAR line_tracker = LineTracker{ Input };

    for (auto chr : Input) {
        line_tracker.next(static_cast<char32_t>(chr));

        if (chr == '\n') {
            BOOST_ASSERT(ExpectedLines.at(current_line) == line_tracker.get());
            ++current_line;
        }
    }

    BOOST_ASSERT(ExpectedLines.at(current_line) == line_tracker.get());
}

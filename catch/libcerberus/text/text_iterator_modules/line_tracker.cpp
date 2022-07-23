#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator_modules/line_tracker.hpp>

using namespace cerb;
using namespace cerb::text;
using namespace cerb::debug;
using namespace cerb::text::module;
using namespace cerb::integral_literals;
using namespace cerb::string_view_literals;

constexpr static auto Input = "Hello, World!\nIt's a \nTest!\nOf line tracker\n "_sv;

constexpr static auto ExpectedLines = std::array{ "Hello, World!", "It's a ", "Test!",
                                                  "Of line "
                                                  "tracker",
                                                  " " };
// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto current_line = 0_ZU;
    auto line_tracker = LineTracker{ Input };

    for (auto chr : Input) {
        line_tracker.next(chr);

        if (chr == '\n') {
            assertEqual(ExpectedLines.at(current_line), line_tracker.get());
            ++current_line;
        }
    };

    assertEqual(ExpectedLines.at(current_line), line_tracker.get());
    return {};
}
();

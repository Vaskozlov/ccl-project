#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator_modules/line_tracker.hpp>

using namespace cerb;
using namespace cerb::text;
using namespace cerb::text::module;
using namespace cerb::string_view_literals;

constexpr static auto Input = u8"Hello, World!\nIt's a \nTest!\nOf line tracker\n "_sv;

constexpr static auto ExpectedLines = std::array{ u8"Hello, World!", u8"It's a ", u8"Test!",
                                                  u8"Of line "
                                                  u8"tracker",
                                                  u8" " };

BOOST_AUTO_TEST_CASE(LineTrackig)
{
    auto current_line = static_cast<size_t>(0);
    auto line_tracker = LineTracker{ Input };

    for (auto chr : Input) {
        line_tracker.next(chr);

        if (chr == '\n') {
            BOOST_ASSERT(ExpectedLines.at(current_line) == line_tracker.get());
            ++current_line;
        }
    }

    BOOST_ASSERT(ExpectedLines.at(current_line) == line_tracker.get());
}

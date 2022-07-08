#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator_modules/line_tracker.hpp>

using namespace cerb::text;
using namespace cerb::integral_literals;

static constexpr std::array<std::string_view, 5> expected_lines = { "Hello, World!", "It's a ",
                                                                    "Test!", "Of line tracker",
                                                                    "" };

constexpr auto testLineTracker() -> bool
{
    auto text_iterator =
        BasicTextIterator<char>{ "Hello, World!\nIt's a \nTest!\nOf line tracker\n" };

    auto index = 0_ZU;
    auto line_tracker = LineTracker<char>{ text_iterator };

    do {
        char chr = text_iterator.nextRawChar();
        line_tracker.next();

        if (chr == '\n') {
            ASSERT_EQ(expected_lines[index], line_tracker.get());
            ++index;
        }
    } while (text_iterator.getCurrentChar() != 0);

    ASSERT_EQ(expected_lines[index], line_tracker.get());
    return {};
}

STATIC_VARIABLE auto result = testLineTracker();

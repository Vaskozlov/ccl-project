#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator_modules/line_tracker.hpp>

using namespace cerb::integral_literals;
using namespace std::string_view_literals;

using LineTracker = cerb::text::LineTracker<char>;
using BasicTextIterator = cerb::text::BasicTextIterator<char>;

// NOLINTNEXTLINE
constexpr auto getExpectedLines() -> std::array<std::string_view, 5>
{
    return { "Hello, World!", "It's a ", "Test!",
             "Of line "
             "tracker",
             "" };
}

STATIC_VARIABLE auto expected_lines = getExpectedLines();

CONSTEXPR_TEST
{
    auto current_line = 0_ZU;
    auto text_iterator = BasicTextIterator{ "Hello, World!\nIt's a \nTest!\nOf line tracker\n" };
    auto line_tracker = LineTracker{ text_iterator };

    do {
        char chr = text_iterator.nextRawChar();
        line_tracker.next();

        if (chr == '\n') {
            ASSERT_EQ(expected_lines.at(current_line), line_tracker.get());
            ++current_line;
        }
    } while (text_iterator.getCurrentChar() != 0);

    ASSERT_EQ(expected_lines.at(current_line), line_tracker.get());

    return {};
}
();

#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator_modules/line_tracker.hpp>

using namespace cerb::text;
using namespace cerb::integral_literals;
using namespace std::string_view_literals;

template<cerb::CharacterLiteral CharT>
    requires(std::is_same_v<char, CharT>)
constexpr auto getExpectedLines() -> std::array<std::string_view, 5>// NOLINT
{
    return { "Hello, World!", "It's a ", "Test!",
             "Of line "
             "tracker",
             "" };
}

template<cerb::CharacterLiteral CharT>
    requires(std::is_same_v<char16_t, CharT>)
constexpr auto getExpectedLines() -> std::array<std::u16string_view, 5>// NOLINT
{
    return { u"Hello, World!", u"It's a ", u"Test!",
             u"Of line "
             u"tracker",
             u"" };
}

template<cerb::CharacterLiteral CharT>
constexpr auto testLineTracker(std::basic_string_view<CharT> input) -> bool
{
    auto current_line = 0_ZU;
    auto text_iterator = BasicTextIterator{ input };
    auto line_tracker = LineTracker{ text_iterator };

    auto expected_lines = getExpectedLines<CharT>();

    do {
        CharT chr = text_iterator.nextRawChar();
        line_tracker.next();

        if (chr == '\n') {
            ASSERT_EQ(expected_lines.at(current_line), line_tracker.get());
            ++current_line;
        }
    } while (text_iterator.getCurrentChar() != 0);

    ASSERT_EQ(expected_lines.at(current_line), line_tracker.get());

    return true;
}

static_assert(testLineTracker("Hello, World!\nIt's a \nTest!\nOf line tracker\n"sv));
static_assert(testLineTracker(u"Hello, World!\nIt's a \nTest!\nOf line tracker\n"sv));

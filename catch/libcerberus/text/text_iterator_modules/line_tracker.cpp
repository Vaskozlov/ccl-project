#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator_modules/line_tracker.hpp>

using namespace cerb::text;
using namespace cerb::debug;
using namespace cerb::integral_literals;
using namespace cerb::string_view_literals;

template<cerb::CharacterLiteral CharT>
constexpr auto getExpectedLines() -> std::array<cerb::string_view, 5>// NOLINT
    requires(std::is_same_v<char, CharT>)
{
    return { "Hello, World!", "It's a ", "Test!",
             "Of line "
             "tracker",
             "" };
}

template<cerb::CharacterLiteral CharT>
constexpr auto getExpectedLines() -> std::array<cerb::u16string_view, 5>// NOLINT
    requires(std::is_same_v<char16_t, CharT>)
{
    return { u"Hello, World!", u"It's a ", u"Test!",
             u"Of line "
             u"tracker",
             u"" };
}

template<cerb::CharacterLiteral CharT>
constexpr auto testLineTracker(cerb::BasicStringView<CharT> input) -> bool
{
    auto current_line = 0_ZU;
    auto text_iterator = BasicTextIterator{ input };
    auto line_tracker = LineTracker{ text_iterator };

    auto expected_lines = getExpectedLines<CharT>();

    do {
        CharT chr = text_iterator.nextRawChar();
        line_tracker.next();

        if (chr == '\n') {
            assertEqual(expected_lines.at(current_line), line_tracker.get());
            ++current_line;
        }
    } while (text_iterator.getCurrentChar() != 0);

    assertEqual(expected_lines.at(current_line), line_tracker.get());

    return true;
}

STRING_TEST
{
    assertTrue(testLineTracker("Hello, World!\nIt's a \nTest!\nOf line tracker\n"_sv));
    assertTrue(testLineTracker(u"Hello, World!\nIt's a \nTest!\nOf line tracker\n"_sv));
    return {};
}
();

#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::debug;
using namespace cerb::integral_literals;
using namespace cerb::string_view_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator{ ""_sv };

    assertEqual(text_iterator.getLine(), 1_ZU);
    assertEqual(text_iterator.getColumn(), 0_ZU);

    text_iterator.nextRawChar();

    assertEqual(text_iterator.getLine(), 1_ZU);
    assertEqual(text_iterator.getColumn(), 0_ZU);

    text_iterator.nextRawChar();

    assertEqual(text_iterator.getLine(), 1_ZU);
    assertEqual(text_iterator.getColumn(), 0_ZU);

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator{ "Hi\nWorld!"_sv };

    assertEqual(text_iterator.getLine(), 1_ZU);
    assertEqual(text_iterator.getColumn(), 0_ZU);

    text_iterator.nextRawChar();

    assertEqual(text_iterator.getLine(), 1_ZU);
    assertEqual(text_iterator.getColumn(), 1_ZU);

    text_iterator.nextRawChar();

    assertEqual(text_iterator.getLine(), 1_ZU);
    assertEqual(text_iterator.getColumn(), 2_ZU);

    text_iterator.nextRawChar();

    assertEqual(text_iterator.getLine(), 2_ZU);
    assertEqual(text_iterator.getColumn(), 1_ZU);

    return {};
}
();

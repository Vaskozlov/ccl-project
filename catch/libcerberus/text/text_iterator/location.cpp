#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::string_view_literals;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8""_sv };

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
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"Hi\n\U0001f000World!"_sv };

    assertEqual(text_iterator.getLine(), 1_ZU);
    assertEqual(text_iterator.getColumn(), 0_ZU);
    assertEqual(text_iterator.getRealColumn(), 0_ZU);

    assertEqual(text_iterator.nextRawChar(), U'H');

    assertEqual(text_iterator.getLine(), 1_ZU);
    assertEqual(text_iterator.getColumn(), 1_ZU);
    assertEqual(text_iterator.getRealColumn(), 1_ZU);

    assertEqual(text_iterator.nextRawChar(), U'i');

    assertEqual(text_iterator.getLine(), 1_ZU);
    assertEqual(text_iterator.getColumn(), 2_ZU);
    assertEqual(text_iterator.getRealColumn(), 2_ZU);

    assertEqual(text_iterator.nextRawChar(), U'\n');

    assertEqual(text_iterator.getLine(), 2_ZU);
    assertEqual(text_iterator.getColumn(), 0_ZU);
    assertEqual(text_iterator.getRealColumn(), 0_ZU);

    assertEqual(text_iterator.nextRawChar(), U'\U0001f000');

    assertEqual(text_iterator.getLine(), 2_ZU);
    assertEqual(text_iterator.getColumn(), 1_ZU);
    assertEqual(text_iterator.getRealColumn(), 4_ZU);

    assertEqual(text_iterator.nextRawChar(), U'W');

    assertEqual(text_iterator.getLine(), 2_ZU);
    assertEqual(text_iterator.getColumn(), 2_ZU);
    assertEqual(text_iterator.getRealColumn(), 5_ZU);// NOLINT

    return {};
}
();

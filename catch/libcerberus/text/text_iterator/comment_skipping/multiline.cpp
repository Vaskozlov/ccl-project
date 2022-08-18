#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"/*", nullptr, { u8"//", u8"/*", u8"*/" } };

    try {
        text_iterator.skipCommentsAndLayout();
        assertTrue(false);
    } catch (const CommentSkipperException &exception) {
        assertEqual(exception.getColumn(), 2_ZU);// NOLINT
        assertEqual(exception.getMessage(), u8"unterminated multiline comment");
    }

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"/**/Hi!", nullptr, { u8"//", u8"/*", u8"*/" } };
    text_iterator.skipCommentsAndLayout();

    assertEqual(text_iterator.nextRawChar(), U'H');

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator =
        TextIterator{ u8"/*1521\n\n151t*/\nHi!", nullptr, { u8"//", u8"/*", u8"*/" } };
    text_iterator.skipCommentsAndLayout();

    assertEqual(text_iterator.nextRawChar(), U'H');

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator =
        TextIterator{ u8"/*1521\n\n151t*/\n/* */ Hi!", nullptr, { u8"//", u8"/*", u8"*/" } };
    text_iterator.skipCommentsAndLayout();

    assertEqual(text_iterator.nextRawChar(), U'H');

    return {};
}
();

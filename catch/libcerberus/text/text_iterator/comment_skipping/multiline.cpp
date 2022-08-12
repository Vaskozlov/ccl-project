#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ "/*", nullptr, { "//", "/*", "*/" } };

    try {
        text_iterator.skipCommentsAndLayout();
        assertTrue(false);
    } catch (const module::CommentSkipperException<char> &exception) {
        assertEqual(exception.getColumn(), 2_ZU);// NOLINT
        assertEqual(exception.getMessage(), "unterminated multiline comment");
    }

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "/**/Hi!", nullptr, { "//", "/*", "*/" } };
    text_iterator.skipCommentsAndLayout();

    assertEqual(text_iterator.getRemaining(), "/Hi!");
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator =
        TextIterator<char>{ "/*1521\n\n151t*/\nHi!", nullptr, { "//", "/*", "*/" } };
    text_iterator.skipCommentsAndLayout();

    assertEqual(text_iterator.getRemaining(), "\nHi!");
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator =
        TextIterator<char>{ "/*1521\n\n151t*/\n/* */ Hi!", nullptr, { "//", "/*", "*/" } };
    text_iterator.skipCommentsAndLayout();

    assertEqual(text_iterator.getRemaining(), " Hi!");
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

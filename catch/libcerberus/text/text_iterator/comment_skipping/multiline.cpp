#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ "/*", nullptr, { "//", "/*", "*/" } };

    ERROR_EXPECTED(
        text_iterator.skipCommentsAndLayout(), BasicTextIteratorException,
        "Error occurred at: , line: 1, column: 2. Error message: unterminated multiline comment\n"
        "/*\n"
        " ^");

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

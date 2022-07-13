#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::debug;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ "/*", { "//", "/*", "*/" } };

    ERROR_EXPECTED(
        text_iterator.skipComments(), BasicTextIteratorException,
        "unterminated "
        "comment");

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "/**/Hi!", { "//", "/*", "*/" } };
    text_iterator.skipComments();

    assertEqual(text_iterator.getRemaining(), "/Hi!");
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "/*1521\n\n151t*/\nHi!", { "//", "/*", "*/" } };
    text_iterator.skipComments();

    assertEqual(text_iterator.getRemaining(), "\nHi!");
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "/*1521\n\n151t*/\n/* */ Hi!", { "//", "/*", "*/" } };
    text_iterator.skipComments();

    assertEqual(text_iterator.getRemaining(), " Hi!");
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

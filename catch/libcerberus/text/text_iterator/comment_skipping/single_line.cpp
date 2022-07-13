#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::debug;

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "//\nHi!", { "//", "/*", "*/" } };
    text_iterator.skipComments();

    assertEqual(text_iterator.getRemaining(), "\nHi!");
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "Hi! //", { "//", "/*", "*/" } };
    text_iterator.rawSkip(3);
    text_iterator.skipComments();

    assertEqual(text_iterator.getRemaining(), "");
    assertEqual(text_iterator.nextRawChar(), '\0');

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "Hi! //\nHello!", { "//", "/*", "*/" } };
    text_iterator.rawSkip(3);
    text_iterator.skipComments();

    assertEqual(text_iterator.getRemaining(), "\nHello!");
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "Hi! //\n //\nHello!", { "//", "/*", "*/" } };
    text_iterator.rawSkip(3);
    text_iterator.skipComments();

    assertEqual(text_iterator.getRemaining(), "\nHello!");
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

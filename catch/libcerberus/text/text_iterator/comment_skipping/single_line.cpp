#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::debug;
using namespace cerb::integral_literals;
using namespace cerb::string_view_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "//\nHi!"_sv, { "//", "/*", "*/" } };
    text_iterator.skipComments();

    assertEqual(text_iterator.getRemaining(), "\nHi!"_sv);
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "Hi! //"_sv, { "//", "/*", "*/" } };
    text_iterator.rawSkip(3);
    text_iterator.skipComments();

    assertEqual(text_iterator.getRemaining(), ""_sv);
    assertEqual(text_iterator.nextRawChar(), '\0');

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "Hi! //\nHello!"_sv, { "//", "/*", "*/" } };
    text_iterator.rawSkip(3);
    text_iterator.skipComments();

    assertEqual(text_iterator.getRemaining(), "\nHello!"_sv);
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "Hi! //\n //\nHello!"_sv, { "//", "/*", "*/" } };
    text_iterator.rawSkip(3);
    text_iterator.skipComments();

    assertEqual(text_iterator.getRemaining(), "\nHello!"_sv);
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

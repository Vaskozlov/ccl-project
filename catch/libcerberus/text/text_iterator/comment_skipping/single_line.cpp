#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "//\nHi!", nullptr, { "//" } };
    text_iterator.skipCommentsAndLayout();

    assertEqual(text_iterator.getRemaining(), "\nHi!");
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "Hi! //", nullptr, { "//" } };
    text_iterator.rawSkip(3);
    text_iterator.skipCommentsAndLayout();

    assertEqual(text_iterator.getRemaining(), "");
    assertEqual(text_iterator.nextRawChar(), '\0');

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "Hi! //\nHello!", nullptr, { "//" } };
    text_iterator.rawSkip(3);
    text_iterator.skipCommentsAndLayout();

    assertEqual(text_iterator.getRemaining(), "\nHello!");
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "Hi! //\n //\nHello!", nullptr, { "//" } };
    text_iterator.rawSkip(3);
    text_iterator.skipCommentsAndLayout();

    assertEqual(text_iterator.getRemaining(), "\nHello!");
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

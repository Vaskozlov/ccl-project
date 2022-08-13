#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"\U0001F600\nHi!", nullptr, { u8"\U0001F600" } };
    text_iterator.skipCommentsAndLayout();

    assertEqual(text_iterator.getRemaining(), u8"\nHi!");
    assertEqual(text_iterator.nextRawChar(), u8'H');

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"Hi! //", nullptr, { u8"//" } };
    text_iterator.rawSkip(3);
    text_iterator.skipCommentsAndLayout();

    assertEqual(text_iterator.getRemaining(), u8"");
    assertEqual(text_iterator.nextRawChar(), u8'\0');

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"Hi! //\nHello!", nullptr, { u8"//" } };
    text_iterator.rawSkip(3);
    text_iterator.skipCommentsAndLayout();

    assertEqual(text_iterator.getRemaining(), u8"\nHello!");
    assertEqual(text_iterator.nextRawChar(), u8'H');

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"Hi! //\n //\nHello!", nullptr, { u8"//" } };
    text_iterator.rawSkip(3);
    text_iterator.skipCommentsAndLayout();

    assertEqual(text_iterator.getRemaining(), u8"\nHello!");
    assertEqual(text_iterator.nextRawChar(), u8'H');

    return {};
}
();

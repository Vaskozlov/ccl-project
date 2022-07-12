#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::debug;
using namespace cerb::integral_literals;
using namespace cerb::string_view_literals;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ "/*"_sv, { "//", "/*", "*/" } };

    ERROR_EXPECTED(
        text_iterator.skipComments(), cerb::BasicTextIteratorException,
        "unterminated "
        "comment");

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "/**/Hi!"_sv, { "//", "/*", "*/" } };
    text_iterator.skipComments();

    assertEqual(text_iterator.getRemaining(), "/Hi!"_sv);
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

// NOLINTNEXTLINE
CONSTEXPR_TEST
{
    auto text_iterator = TextIterator<char>{ "/*1521\n\n151t*/\nHi!"_sv, { "//", "/*", "*/" } };
    text_iterator.skipComments();

    assertEqual(text_iterator.getRemaining(), "\nHi!"_sv);
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

// NOLINTNEXTLINE
CONSTEXPR_TEST
{
    auto text_iterator =
        TextIterator<char>{ "/*1521\n\n151t*/\n/* */ Hi!"_sv, { "//", "/*", "*/" } };
    text_iterator.skipComments();

    assertEqual(text_iterator.getRemaining(), " Hi!"_sv);
    assertEqual(text_iterator.nextRawChar(), 'H');

    return {};
}
();

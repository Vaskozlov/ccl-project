#include <cerberus/debug/debug_file.hpp>
#include <cerberus/utf8.hpp>
#include <string>

// NOLINTNEXTLINE
STRING_TEST
{
    auto test = std::u8string{};

    cerb::utf8::appendUtf32ToUtf8Container(test, U'\u00FF');

    assertEqual(test, u8"\u00FF");

    test.clear();
    cerb::utf8::appendUtf32ToUtf8Container(test, U'\uFFFF');
    assertEqual(test, u8"\uFFFF");

    test.clear();
    cerb::utf8::appendUtf32ToUtf8Container(test, U'\U0010FFFF');
    assertEqual(test, u8"\U0010FFFF");

    return {};
}
();

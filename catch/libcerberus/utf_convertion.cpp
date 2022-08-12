#include <cerberus/debug/debug_file.hpp>
#include <cerberus/utf8.hpp>

// NOLINTNEXTLINE
STRING_TEST
{
    std::u8string test;

    cerb::utf8::appendUtf32ToUtf8Container(test, U'\u00FF');
    assertEqual(test, u8"\xC3\xBF");

    test.clear();
    cerb::utf8::appendUtf32ToUtf8Container(test, U'\uFFFF');
    assertEqual(test, u8"\xEF\xBF\xBF");

    test.clear();
    cerb::utf8::appendUtf32ToUtf8Container(test, U'\U0010FFFF');
    assertEqual(test, u8"\xF4\x8F\xBF\xBF");

    return {};
}
();

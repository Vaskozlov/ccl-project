#include <ccl/debug/debug_file.hpp>
#include <ccl/utf8.hpp>
#include <string>

using namespace ccl;

BOOST_AUTO_TEST_CASE(OneBytesUnicodeConvertion)
{
    auto test = std::string{};
    utf8::appendUtf32ToUtf8Container(test, U'\u000F');
    BOOST_ASSERT(test == "\u000F");
}

BOOST_AUTO_TEST_CASE(TwoBytesUnicodeConvertion)
{
    auto test = std::string{};
    utf8::appendUtf32ToUtf8Container(test, U'\u00FF');
    BOOST_ASSERT(test == "\u00FF");
}

BOOST_AUTO_TEST_CASE(TreeBytesUnicodeConvertion)
{
    auto test = std::string{};
    utf8::appendUtf32ToUtf8Container(test, U'\uFFFF');
    BOOST_ASSERT(test == "\uFFFF");
}

BOOST_AUTO_TEST_CASE(FourBytesUnicodeConvertion)
{
    auto test = std::string{};
    utf8::appendUtf32ToUtf8Container(test, U'\U0010FFFF');
    BOOST_ASSERT(test == "\U0010FFFF");
}

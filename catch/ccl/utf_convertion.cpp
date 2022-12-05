#include <ccl/debug/debug_file.hpp>
#include <ccl/utf8.hpp>

using namespace ccl;

BOOST_AUTO_TEST_SUITE(UnicodeConvertion)

BOOST_AUTO_TEST_CASE(OneBytesUnicodeConvertion)
{
    auto test = std::string{};
    utf8::appendUtf32ToUtf8Container(test, U'\u000F');
    BOOST_ASSERT(test == "\u000F");

    test.clear();
    utf8::appendUtf32ToUtf8Container(test, 'A');
    BOOST_ASSERT(test == "A");
}

BOOST_AUTO_TEST_CASE(TwoBytesUnicodeConvertion)
{
    auto test = std::string{};
    utf8::appendUtf32ToUtf8Container(test, U'\u00FD');
    BOOST_ASSERT(test == "\u00FD");
}

BOOST_AUTO_TEST_CASE(TreeBytesUnicodeConvertion)
{
    auto test = std::string{};
    utf8::appendUtf32ToUtf8Container(test, U'\uFCAF');
    BOOST_ASSERT(test == "\uFCAF");
}

BOOST_AUTO_TEST_CASE(FourBytesUnicodeConvertion)
{
    auto test = std::string{};
    utf8::appendUtf32ToUtf8Container(test, U'\U0010FDFE');
    BOOST_ASSERT(test == "\U0010FDFE");
}

BOOST_AUTO_TEST_SUITE_END()

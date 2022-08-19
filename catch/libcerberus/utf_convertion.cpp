#include <boost/test/unit_test.hpp>
#include <cerberus/utf8.hpp>
#include <string>

BOOST_AUTO_TEST_CASE(UtfConvertion)
{
    auto test = std::u8string{};

    cerb::utf8::appendUtf32ToUtf8Container(test, U'\u00FF');

    BOOST_ASSERT(test == u8"\u00FF");

    test.clear();
    cerb::utf8::appendUtf32ToUtf8Container(test, U'\uFFFF');
    BOOST_ASSERT(test == u8"\uFFFF");

    test.clear();
    cerb::utf8::appendUtf32ToUtf8Container(test, U'\U0010FFFF');
    BOOST_ASSERT(test == u8"\U0010FFFF");
}

#include <ccl/char.hpp>
#include <ccl/debug/debug_file.hpp>

using namespace ccl;
using namespace integral_literals;

static constexpr auto HCTI = HexadecimalCharsToInt<char>;

// NOLINTBEGIN

BOOST_AUTO_TEST_CASE(BasicHexadecimalCharsToIntTest)
{
    BOOST_STATIC_ASSERT(HCTI.at('0') == 0);
    BOOST_STATIC_ASSERT(HCTI.at('1') == 1);
    BOOST_STATIC_ASSERT(HCTI.at('2') == 2);
    BOOST_STATIC_ASSERT(HCTI.at('3') == 3);
    BOOST_STATIC_ASSERT(HCTI.at('4') == 4);
    BOOST_STATIC_ASSERT(HCTI.at('5') == 5);
    BOOST_STATIC_ASSERT(HCTI.at('6') == 6);
    BOOST_STATIC_ASSERT(HCTI.at('7') == 7);
    BOOST_STATIC_ASSERT(HCTI.at('8') == 8);
    BOOST_STATIC_ASSERT(HCTI.at('9') == 9);

    BOOST_STATIC_ASSERT(HCTI.at('A') == 10);
    BOOST_STATIC_ASSERT(HCTI.at('B') == 11);
    BOOST_STATIC_ASSERT(HCTI.at('C') == 12);
    BOOST_STATIC_ASSERT(HCTI.at('D') == 13);
    BOOST_STATIC_ASSERT(HCTI.at('E') == 14);
    BOOST_STATIC_ASSERT(HCTI.at('F') == 15);

    BOOST_STATIC_ASSERT(HCTI.at('a') == 10);
    BOOST_STATIC_ASSERT(HCTI.at('b') == 11);
    BOOST_STATIC_ASSERT(HCTI.at('c') == 12);
    BOOST_STATIC_ASSERT(HCTI.at('d') == 13);
    BOOST_STATIC_ASSERT(HCTI.at('e') == 14);
    BOOST_STATIC_ASSERT(HCTI.at('f') == 15);
}

// NOLINTEND

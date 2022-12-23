#include <ccl/char.hpp>
#include <ccl/debug/debug_file.hpp>

using namespace ccl;
using namespace integral_literals;

static constexpr auto HCTI = HexadecimalCharsToInt<char>;

// NOLINTBEGIN

BOOST_AUTO_TEST_CASE(BasicHexadecimalCharsToIntTest)
{
    BOOST_STATIC_ASSERT(HCTI['0'] == 0);
    BOOST_STATIC_ASSERT(HCTI['1'] == 1);
    BOOST_STATIC_ASSERT(HCTI['2'] == 2);
    BOOST_STATIC_ASSERT(HCTI['3'] == 3);
    BOOST_STATIC_ASSERT(HCTI['4'] == 4);
    BOOST_STATIC_ASSERT(HCTI['5'] == 5);
    BOOST_STATIC_ASSERT(HCTI['6'] == 6);
    BOOST_STATIC_ASSERT(HCTI['7'] == 7);
    BOOST_STATIC_ASSERT(HCTI['8'] == 8);
    BOOST_STATIC_ASSERT(HCTI['9'] == 9);

    BOOST_STATIC_ASSERT(HCTI['A'] == 10);
    BOOST_STATIC_ASSERT(HCTI['B'] == 11);
    BOOST_STATIC_ASSERT(HCTI['C'] == 12);
    BOOST_STATIC_ASSERT(HCTI['D'] == 13);
    BOOST_STATIC_ASSERT(HCTI['E'] == 14);
    BOOST_STATIC_ASSERT(HCTI['F'] == 15);

    BOOST_STATIC_ASSERT(HCTI['a'] == 10);
    BOOST_STATIC_ASSERT(HCTI['b'] == 11);
    BOOST_STATIC_ASSERT(HCTI['c'] == 12);
    BOOST_STATIC_ASSERT(HCTI['d'] == 13);
    BOOST_STATIC_ASSERT(HCTI['e'] == 14);
    BOOST_STATIC_ASSERT(HCTI['f'] == 15);
}

// NOLINTEND

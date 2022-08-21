#include <cerberus/char.hpp>
#include <cerberus/debug/debug_file.hpp>

using namespace cerb;
using namespace cerb::integral_literals;

template<CharacterLiteral CharT>
static constexpr auto HCTI = HexadecimalCharsToInt<CharT>;

// NOLINTBEGIN

BOOST_AUTO_TEST_CASE(BasicHexadecimalCharsToIntTest)
{
    BOOST_STATIC_ASSERT(HCTI<char>.at('0') == 0_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('1') == 1_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('2') == 2_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('3') == 3_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('4') == 4_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('5') == 5_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('6') == 6_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('7') == 7_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('8') == 8_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('9') == 9_U16);

    BOOST_STATIC_ASSERT(HCTI<char>.at('A') == 10_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('B') == 11_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('C') == 12_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('D') == 13_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('E') == 14_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('F') == 15_U16);

    BOOST_STATIC_ASSERT(HCTI<char>.at('a') == 10_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('b') == 11_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('c') == 12_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('d') == 13_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('e') == 14_U16);
    BOOST_STATIC_ASSERT(HCTI<char>.at('f') == 15_U16);
}

BOOST_AUTO_TEST_CASE(Utf16HexadecimalCharsToIntTest)
{
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'0') == 0_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'1') == 1_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'2') == 2_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'3') == 3_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'4') == 4_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'5') == 5_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'6') == 6_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'7') == 7_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'8') == 8_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'9') == 9_U16);

    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'A') == 10_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'B') == 11_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'C') == 12_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'D') == 13_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'E') == 14_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'F') == 15_U16);

    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'a') == 10_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'b') == 11_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'c') == 12_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'd') == 13_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'e') == 14_U16);
    BOOST_STATIC_ASSERT(HCTI<char16_t>.at(u'f') == 15_U16);
}

// NOLINTEND

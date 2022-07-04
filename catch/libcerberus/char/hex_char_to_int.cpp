#include <cerberus/char.hpp>
#include <cerberus/debug/debug.hpp>

using namespace cerb;
using namespace integral_literals;

STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('0'), 0_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('1'), 1_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('2'), 2_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('3'), 3_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('4'), 4_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('5'), 5_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('6'), 6_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('7'), 7_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('8'), 8_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('9'), 9_u16);

STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('A'), 10_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('B'), 11_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('C'), 12_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('D'), 13_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('E'), 14_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('F'), 15_u16);

STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('a'), 10_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('b'), 11_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('c'), 12_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('d'), 13_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('e'), 14_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char>.at('f'), 15_u16);

STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'0'), 0_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'1'), 1_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'2'), 2_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'3'), 3_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'4'), 4_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'5'), 5_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'6'), 6_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'7'), 7_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'8'), 8_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'9'), 9_u16);

STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'A'), 10_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'B'), 11_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'C'), 12_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'D'), 13_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'E'), 14_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'F'), 15_u16);

STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'a'), 10_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'b'), 11_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'c'), 12_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'd'), 13_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'e'), 14_u16);
STATIC_ASSERT_EQ(HexadecimalCharsToInt<char16_t>.at(u'f'), 15_u16);

#include <ccl/char.hpp>
#include <ccl/debug/debug.hpp>

// NOLINTBEGIN

TEST_CASE("BasicHexadecimalCharsToIntTest", "[HexadecimalChars]")
{
    static constexpr auto HCTI = ccl::HexadecimalCharsToInt;

    REQUIRE(HCTI.at('0') == 0);
    REQUIRE(HCTI.at('1') == 1);
    REQUIRE(HCTI.at('2') == 2);
    REQUIRE(HCTI.at('3') == 3);
    REQUIRE(HCTI.at('4') == 4);
    REQUIRE(HCTI.at('5') == 5);
    REQUIRE(HCTI.at('6') == 6);
    REQUIRE(HCTI.at('7') == 7);
    REQUIRE(HCTI.at('8') == 8);
    REQUIRE(HCTI.at('9') == 9);

    REQUIRE(HCTI.at('A') == 10);
    REQUIRE(HCTI.at('B') == 11);
    REQUIRE(HCTI.at('C') == 12);
    REQUIRE(HCTI.at('D') == 13);
    REQUIRE(HCTI.at('E') == 14);
    REQUIRE(HCTI.at('F') == 15);

    REQUIRE(HCTI.at('a') == 10);
    REQUIRE(HCTI.at('b') == 11);
    REQUIRE(HCTI.at('c') == 12);
    REQUIRE(HCTI.at('d') == 13);
    REQUIRE(HCTI.at('e') == 14);
    REQUIRE(HCTI.at('f') == 15);
}

// NOLINTEND

#ifndef CCL_PROJECT_CHAR_HPP
#define CCL_PROJECT_CHAR_HPP

#include <ccl/flatmap.hpp>

namespace ccl
{
    constexpr inline auto HexadecimalCharsToInt = StaticFlatmap<char32_t, u16, 22>(
        {{'0', 0},  {'1', 1},  {'2', 2},  {'3', 3},  {'4', 4},  {'5', 5},  {'6', 6},  {'7', 7},
         {'8', 8},  {'9', 9},  {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15},
         {'A', 10}, {'B', 11}, {'C', 12}, {'D', 13}, {'E', 14}, {'F', 15}});

    CCL_DECL auto isEoF(CharacterLiteral auto chr) noexcept -> bool
    {
        return chr == '\0';
    }

    CCL_DECL auto isLayout(CharacterLiteral auto chr) noexcept -> bool
    {
        return land(chr > '\0', chr <= ' ');
    }

    CCL_DECL auto isLayoutOrEoF(CharacterLiteral auto chr) noexcept -> bool
    {
        return land(chr >= '\0', chr <= ' ');
    }

    CCL_DECL auto isDigit(CharacterLiteral auto chr) noexcept -> bool
    {
        return land(chr >= '0', chr <= '9');
    }
}// namespace ccl

#endif /* CCL_PROJECT_CHAR_HPP */

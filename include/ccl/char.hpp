#ifndef CCL_PROJECT_CHAR_HPP
#define CCL_PROJECT_CHAR_HPP

#include <ccl/ccl.hpp>
#include <isl/static_flat_map.hpp>

namespace ccl
{
    constexpr inline auto HexadecimalCharsToInt = isl::StaticFlatmap<char32_t, u16, 22>({
        {'0', 0 },
        {'1', 1 },
        {'2', 2 },
        {'3', 3 },
        {'4', 4 },
        {'5', 5 },
        {'6', 6 },
        {'7', 7 },
        {'8', 8 },
        {'9', 9 },
        {'a', 10},
        {'b', 11},
        {'c', 12},
        {'d', 13},
        {'e', 14},
        {'f', 15},
        {'A', 10},
        {'B', 11},
        {'C', 12},
        {'D', 13},
        {'E', 14},
        {'F', 15}
    });

    CCL_DECL auto isEoF(isl::CharacterLiteral auto chr) noexcept -> bool
    {
        return chr == '\0';
    }

    CCL_DECL auto isLayout(isl::CharacterLiteral auto chr) noexcept -> bool
    {
        return chr > '\0' && chr <= ' ';
    }

    CCL_DECL auto isLayoutOrEoF(isl::CharacterLiteral auto chr) noexcept -> bool
    {
        return chr >= '\0' && chr <= ' ';
    }

    CCL_DECL auto isDigit(isl::CharacterLiteral auto chr) noexcept -> bool
    {
        return chr >= '0' && chr <= '9';
    }
} // namespace ccl

#endif /* CCL_PROJECT_CHAR_HPP */

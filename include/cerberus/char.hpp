#ifndef CERBERUS_PROJECT_CHAR_HPP
#define CERBERUS_PROJECT_CHAR_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/flatmap.hpp>

namespace cerb
{
    template<CharacterLiteral CharT>
    constexpr auto HexadecimalCharsToInt =
        Flatmap<CharT, u16, 22>{ { '0', 0 },  { '1', 1 },  { '2', 2 },  { '3', 3 },  { '4', 4 },
                                 { '5', 5 },  { '6', 6 },  { '7', 7 },  { '8', 8 },  { '9', 9 },
                                 { 'a', 10 }, { 'b', 11 }, { 'c', 12 }, { 'd', 13 }, { 'e', 14 },
                                 { 'f', 15 }, { 'A', 10 }, { 'B', 11 }, { 'C', 12 }, { 'D', 13 },
                                 { 'E', 14 }, { 'F', 15 } };

    template<CharacterLiteral CharT>
    constexpr auto IntToHexadecimalChars =
        std::array<CharT, 16>{ '0', '1', '2', '3', '4', '5', '6', '7',
                               '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isEoF(CharT chr) -> bool
    {
        return chr == static_cast<CharT>(0);
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLayout(CharT chr) -> bool
    {
        return land(chr > static_cast<CharT>(0), chr <= ' ');
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isDigit(CharT chr) -> bool
    {
        return land(chr >= '0', chr <= '9');
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isUcLetter(CharT chr) -> bool
    {
        return land(chr >= 'A', chr <= 'Z');
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLcLetter(CharT chr) -> bool
    {
        return land(chr >= 'a', chr <= 'z');
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLetter(CharT chr) -> bool
    {
        return lor(isUcLetter(chr), isLcLetter(chr));
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isLetterOrDigit(CharT chr) -> bool
    {
        return lor(isLetter(chr), isDigit(chr));
    }
}// namespace cerb

#endif /* CERBERUS_PROJECT_CHAR_HPP */

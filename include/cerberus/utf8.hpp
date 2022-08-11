#ifndef CERBERUS_PROJECT_UTF8_HPP
#define CERBERUS_PROJECT_UTF8_HPP

#include <cerberus/cerberus.hpp>

namespace cerb::utf8
{
    CERBLIB_EXCEPTION(Utf8ConvertionError, CerberusException);

    constexpr u8 one_byte_mask = 0b1000'0000;
    constexpr u8 two_bytes_mask = 0b1110'0000;
    constexpr u8 two_bytes_signature = 0b1100'0000;
    constexpr u8 tree_bytes_mask = 0b1111'0000;
    constexpr u8 tree_bytes_signature = 0b1110'0000;
    constexpr u8 four_bytes_mask = 0b1111'1000;
    constexpr u8 four_bytes_signature = 0b1111'0000;
    constexpr u8 continuation_mask = 0b1100'0000;
    constexpr u8 continuation_signature = 0b1000'0000;
    constexpr u8 trailing_bytes_size = 6;

    constexpr u32 one_byte_utf_max = 127;
    constexpr u32 two_byte_utf_max = 2047;
    constexpr u32 tree_byte_utf_max = 65535;
    constexpr u32 four_byte_utf_max = 1114111;

    CERBLIB_DECL auto isTrailingCharacter(char8_t byte) -> bool
    {
        return (byte & continuation_mask) == continuation_signature;
    }

    CERBLIB_DECL auto isOneByteSize(char8_t chr) -> bool
    {
        return (chr & one_byte_mask) == 0;
    }

    CERBLIB_DECL auto isTwoBytesSize(char8_t chr) -> bool
    {
        return (chr & two_bytes_mask) == two_bytes_signature;
    }

    CERBLIB_DECL auto isThreeBytesSize(char8_t chr) -> bool
    {
        return (chr & tree_bytes_mask) == tree_bytes_signature;
    }

    CERBLIB_DECL auto isFourBytesSize(char8_t chr) -> bool
    {
        return (chr & four_bytes_mask) == four_bytes_signature;
    }

    CERBLIB_DECL auto getMask(u16 size) -> u8
    {
        switch (size) {
        case 1:
            return one_byte_mask;
        case 2:
            return two_bytes_mask;
        case 3:
            return tree_bytes_mask;
        case 4:
            return four_bytes_mask;
        default:
            return 0;
        }
    }

    CERBLIB_DECL auto utfSize(char8_t chr) -> u16
    {
        if (isOneByteSize(chr)) {
            return 1U;
        }

        if (isTwoBytesSize(chr)) {
            return 2U;
        }

        if (isThreeBytesSize(chr)) {
            return 3U;
        }

        if (isFourBytesSize(chr)) {
            return 4U;
        }

        return 0U;
    }

    constexpr auto checkTrailingCharacterMask(char8_t chr) -> void
    {
        using namespace std::string_view_literals;

        if ((chr & continuation_mask) != continuation_signature) {
            throw Utf8ConvertionError{ "unable to convert symbol to utf8"sv };
        }
    }

    constexpr auto addUtf32ToUtf8String(std::u8string &string, char32_t chr) -> void
    {
        using namespace std::string_view_literals;

        // NOLINTBEGIN

        if (chr <= one_byte_utf_max) {
            string.push_back(static_cast<char8_t>(chr));
        } else if (chr <= two_byte_utf_max) {
            string.push_back(static_cast<char8_t>(two_bytes_signature | (chr >> 6)));
            string.push_back(static_cast<char8_t>(continuation_signature | (chr & 0b0011'1111)));
        } else if (chr <= tree_byte_utf_max) {
            string.push_back(static_cast<char8_t>(tree_bytes_signature | (chr >> 12)));
            string.push_back(
                static_cast<char8_t>(continuation_signature | ((chr >> 6) & 0b0011'1111)));
            string.push_back(static_cast<char8_t>(continuation_signature | (chr & 0b0011'1111)));
        } else if (chr <= four_byte_utf_max) {
            string.push_back(static_cast<char8_t>(four_bytes_signature | (chr >> 18)));
            string.push_back(
                static_cast<char8_t>(continuation_signature | ((chr >> 12) & 0b0011'1111)));
            string.push_back(
                static_cast<char8_t>(continuation_signature | ((chr >> 6) & 0b0011'1111)));
            string.push_back(static_cast<char8_t>(continuation_signature | (chr & 0b0011'1111)));
        } else {
            throw Utf8ConvertionError{ "unable to convert symbol to utf8"sv };
        }

        // NOLINTEND
    }
}// namespace cerb::utf8

#endif /* CERBERUS_PROJECT_UTF8_HPP */

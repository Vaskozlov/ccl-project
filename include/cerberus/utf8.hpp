#ifndef CERBERUS_PROJECT_UTF8_HPP
#define CERBERUS_PROJECT_UTF8_HPP

#include <cerberus/cerberus.hpp>

namespace cerb::utf8
{
    CERBLIB_EXCEPTION(Utf8ConvertionError, CerberusException);

    constexpr u8 ascii_character_mask = 0b1000'0000;
    constexpr u8 two_bytes_mask = 0b1110'0000;
    constexpr u8 two_bytes_signature = 0b1100'0000;
    constexpr u8 tree_bytes_mask = 0b1111'0000;
    constexpr u8 tree_bytes_signature = 0b1110'0000;
    constexpr u8 four_bytes_mask = 0b1111'1000;
    constexpr u8 four_bytes_signature = 0b1111'0000;
    constexpr u8 continuation_mask = 0b1100'0000;
    constexpr u8 continuation_signature = 0b1000'0000;
    constexpr u8 trailing_bytes_size = 6;

    CERBLIB_DECL auto isOneByteSize(char8_t chr) -> bool
    {
        return (chr & ascii_character_mask) == 0;
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

    constexpr auto checkTrailingCharacterMask(char8_t chr) -> void
    {
        using namespace std::string_view_literals;

        if ((chr & continuation_mask) != continuation_signature) {
            throw Utf8ConvertionError{ "unable to convert symbol to utf8"sv };
        }
    }
}// namespace cerb::utf8

#endif /* CERBERUS_PROJECT_UTF8_HPP */

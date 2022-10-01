#ifndef CCL_PROJECT_UTF8_HPP
#define CCL_PROJECT_UTF8_HPP

#include <ccl/ccl.hpp>

namespace ccl::utf8
{
    CCL_EXCEPTION(Utf8ConvertionError, CclException);

    template<typename T>
    concept ValueTypeUtf8 = std::is_same_v<char, typename T::value_type>;

    constexpr u32 OneByteMax = 127;
    constexpr u32 TwoBytesMax = 2047;
    constexpr u32 TreeBytesMax = 65535;
    constexpr u32 FourBytesMax = 1114111;

    constexpr u8 OneByteMask = 0b1000'0000;
    constexpr u8 TwoBytesMask = 0b1110'0000;
    constexpr u8 TwoBytesSignature = 0b1100'0000;
    constexpr u8 TreeBytesMask = 0b1111'0000;
    constexpr u8 TreeBytesSignature = 0b1110'0000;
    constexpr u8 FourBytesMask = 0b1111'1000;
    constexpr u8 FourBytesSignature = 0b1111'0000;
    constexpr u8 ContinuationMask = 0b1100'0000;
    constexpr u8 ContinuationSignature = 0b1000'0000;
    constexpr u8 TrailingSize = 6;

    constexpr std::array<u8, 5> UtfMasks{ 0, OneByteMask, TwoBytesMask, TreeBytesMask,
                                          FourBytesMask };

    CCL_DECL CCL_INLINE auto isTrailingCharacter(char chr) noexcept -> bool
    {
        return (chr & ContinuationMask) == ContinuationSignature;
    }

    CCL_DECL CCL_INLINE auto isOneByteSize(char chr) noexcept -> bool
    {
        return (chr & OneByteMask) == 0;
    }

    CCL_DECL CCL_INLINE auto isTwoBytesSize(char chr) noexcept -> bool
    {
        return (chr & TwoBytesMask) == TwoBytesSignature;
    }

    CCL_DECL CCL_INLINE auto isThreeBytesSize(char chr) noexcept -> bool
    {
        return (chr & TreeBytesMask) == TreeBytesSignature;
    }

    CCL_DECL CCL_INLINE auto isFourBytesSize(char chr) noexcept -> bool
    {
        return (chr & FourBytesMask) == FourBytesSignature;
    }

    CCL_DECL auto getMask(u16 size) -> u8
    {
        return UtfMasks.at(size);
    }

    CCL_DECL auto utfSize(char chr) noexcept -> u16
    {
        if (isOneByteSize(chr)) [[likely]] {
            return 1U;
        }

        if (isTwoBytesSize(chr)) [[unlikely]] {
            return 2U;
        }

        if (isThreeBytesSize(chr)) [[unlikely]] {
            return 3U;
        }

        if (isFourBytesSize(chr)) [[unlikely]] {
            return 4U;
        }

        return 0U;
    }

    constexpr auto checkTrailingCharacterMask(char chr) -> void
    {
        using namespace std::string_view_literals;

        if ((chr & ContinuationMask) != ContinuationSignature) {
            throw Utf8ConvertionError{ "unable to convert symbol to utf8"sv };
        }
    }

    template<ValueTypeUtf8 T>
    constexpr auto appendUtf32ToUtf8Container(T &string, char32_t chr) -> void
    {
        using namespace std::string_view_literals;

        auto &&cast = [](auto value) { return static_cast<char>(value); };

        // NOLINTBEGIN

        if (chr <= OneByteMax) [[likely]] {
            string.push_back(cast(chr));
        } else if (chr <= TwoBytesMax) {
            string.push_back(cast(TwoBytesSignature | (chr >> 6)));
            string.push_back(cast(ContinuationSignature | (chr & 0b0011'1111)));
        } else if (chr <= TreeBytesMax) {
            string.push_back(cast(TreeBytesSignature | (chr >> 12)));
            string.push_back(cast(ContinuationSignature | ((chr >> 6) & 0b0011'1111)));
            string.push_back(cast(ContinuationSignature | (chr & 0b0011'1111)));
        } else if (chr <= FourBytesMax) {
            string.push_back(cast(FourBytesSignature | (chr >> 18)));
            string.push_back(cast(ContinuationSignature | ((chr >> 12) & 0b0011'1111)));
            string.push_back(cast(ContinuationSignature | ((chr >> 6) & 0b0011'1111)));
            string.push_back(cast(ContinuationSignature | (chr & 0b0011'1111)));
        } else [[unlikely]] {
            throw Utf8ConvertionError{ "unable to convert symbol to utf8"sv };
        }

        // NOLINTEND
    }
}// namespace ccl::utf8


#endif /* CCL_PROJECT_UTF8_HPP */

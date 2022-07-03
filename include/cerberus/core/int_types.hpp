#ifndef CERBERUS_PROJECT_INT_TYPES_HPP
#define CERBERUS_PROJECT_INT_TYPES_HPP

#include <cinttypes>

namespace cerb
{
    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;

    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;

    using ssize_t = intmax_t;

    namespace integral_literals
    {
        constexpr auto operator"" _u8(unsigned long long value) -> u8
        {
            return static_cast<u8>(value);
        }

        constexpr auto operator"" _u16(unsigned long long value) -> u16
        {
            return static_cast<u16>(value);
        }

        constexpr auto operator"" _u32(unsigned long long value) -> u32
        {
            return static_cast<u32>(value);
        }

        constexpr auto operator"" _u64(unsigned long long value) -> u64
        {
            return static_cast<u64>(value);
        }
    }// namespace integral_literals
}// namespace cerb

#endif /* CERBERUS_PROJECT_INT_TYPES_HPP */

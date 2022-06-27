#ifndef CERBERUS_PROJECT_INT_TYPES_HPP
#define CERBERUS_PROJECT_INT_TYPES_HPP

#include <cinttypes>

namespace cerb
{
    using i8 = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;

    using u8 = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

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

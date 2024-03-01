module;
#include <isl/core/defines.hpp>
export module ccl.core;

export import isl;

export namespace ccl
{
    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;

    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;

    using Id = std::size_t;
    using ssize_t = std::intmax_t;

    using f32 = std::float_t;
    using f64 = std::double_t;

    ISL_EXCEPTION(UnrecoverableError, std::runtime_error, runtime_error);
}// namespace ccl

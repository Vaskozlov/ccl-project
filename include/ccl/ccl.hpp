#ifndef CCL_PROJECT_CCL_HPP
#define CCL_PROJECT_CCL_HPP

#include <algorithm>
#include <array>

#include <isl/isl.hpp>

#include <fmt/color.h>
#include <fmt/compile.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#define CCL_DECL ISL_DECL
#define CCL_INLINE ISL_INLINE
#define CCL_LIFETIMEBOUND ISL_LIFETIMEBOUND
#define CCL_TRIVIAL_ABI ISL_TRIVIAL_ABI
#define CCL_NOEXCEPT_IF(EXPR) ISL_NOEXCEPT_IF(EXPR)

#define CCL_REPEAT_WHILE(X)                                                                        \
    while (X) {}

#define CCL_REPEAT_FOREVER(X)                                                                      \
    while (true) {                                                                                 \
        X;                                                                                         \
    }

namespace ccl
{
    // system must be at least 64 bit
    // NOLINTNEXTLINE
    static_assert(sizeof(std::size_t) >= 8);

    using i8 = isl::i8;
    using i16 = isl::i16;
    using i32 = isl::i32;
    using i64 = isl::i64;

    using u8 = isl::u8;
    using u16 = isl::u16;
    using u32 = isl::u32;
    using u64 = isl::u64;

    using Id = isl::Id;

    using f32 = isl::f32;
    using f64 = isl::f64;

    ISL_EXCEPTION(UnrecoverableError, std::runtime_error, runtime_error);
}// namespace ccl

#endif /* CCL_PROJECT_CCL_HPP */

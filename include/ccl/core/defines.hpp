#ifndef CCL_PROJECT_DEFINES_HPP
#define CCL_PROJECT_DEFINES_HPP

#include <cassert>
#include <cinttypes>
#include <cstddef>
#include <version>

#define CCL_DECL [[nodiscard]] constexpr

// NOLINTBEGIN

#define CCL_PERFECT_FORWARDING(Repr, Type)                                                         \
    template<typename Repr>                                                                        \
        requires std::is_convertible_v<Repr, Type>

#define CCL_PERFECT_FORWARDING_2(Repr1, Type1, Repr2, Type2)                                       \
    template<typename Repr1, typename Repr2>                                                       \
        requires std::is_convertible_v<Repr1, Type1> && std::is_convertible_v<Repr2, Type2>

#define CCL_PERFECT_FORWARDING_3(Repr1, Type1, Repr2, Type2, Repr3, Type3)                         \
    template<typename Repr1, typename Repr2, typename Repr3>                                       \
        requires std::is_convertible_v<Repr1, Type1> && std::is_convertible_v<Repr2, Type2> &&     \
                 std::is_convertible_v<Repr3, Type3>

// NOLINTEND

#define CCL_STRX(x) #x
#define CCL_STR(x) CCL_STRX(x)

#define CCL_ASSERT(x) assert(x)
#define CCL_ASSERT_MSG(x, message) assert((x) && message)

#if INTPTR_MAX == INT32_MAX
#    define CCL_64BIT false
#else
#    define CCL_64BIT true
#endif /* CCL_64BIT */

#if __cpp_if_consteval
#    define CCL_COMPILE_TIME_BRANCH consteval
#else
#    define CCL_COMPILE_TIME_BRANCH (std::is_constant_evaluated())
#endif /* CCL_COMPILE_TIME_BRANCH */

#if __cpp_if_consteval
#    define CCL_RUNTIME_BRANCH !consteval
#else
#    define CCL_RUNTIME_BRANCH (!std::is_constant_evaluated())
#endif /* CCL_RUNTIME_BRANCH */

#if defined(_MSC_VER)
#    define CCL_INLINE __forceinline
#else
#    define CCL_INLINE __attribute__((always_inline)) inline
#endif

#if defined(__clang__)
#    define CCL_TRIVIAL_ABI [[clang::trivial_abi]]
#else
#    define CCL_TRIVIAL_ABI
#endif /* CCL_TRIVIAL_ABI */

#if defined(__clang__)
#    define CCL_UNROLL_N(N) _Pragma(CCL_STR(unroll N))
#elif defined(__GNUC__)
#    define CCL_UNROLL_N(N) _Pragma(CCL_STR(GCC unroll N))
#else
#    define CCL_UNROLL_N(N)
#endif /* CCL_UNROLL_N */

#ifdef __has_builtin
#    if __has_builtin(__builtin_prefetch)
#        define CCL_PREFETCH(ADDR) __builtin_prefetch(ADDR)
#    else
#        define CCL_PREFETCH(ADDR)
#    endif
#else
#    define CCL_PREFETCH(ADDR)
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#    define CCL_LIFETIMEBOUND [[msvc::lifetimebound]]
#elif defined(__clang__)
#    define CCL_LIFETIMEBOUND [[clang::lifetimebound]]
#elif defined(__GNUC__)
#    define CCL_LIFETIMEBOUND__attribute__((lifetimebound))
#else
#    define CCL_LIFETIMEBOUND
#endif

#define CCL_NOEXCEPT_IF(EXPR) noexcept(noexcept(EXPR))

namespace ccl
{
    [[noreturn]] CCL_INLINE auto unreachable() -> void
    {
#if defined(__has_builtin) && __has_builtin(__builtin_unreachable)
        __builtin_unreachable();
#elif defined(_MSC_VER)
        __assume(false);
#else
        assert(false);
#endif
    }
}// namespace ccl

#endif /* CCL_PROJECT_DEFINES_HPP */

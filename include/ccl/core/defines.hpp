#ifndef CCL_PROJECT_DEFINES_HPP
#define CCL_PROJECT_DEFINES_HPP

#include <cassert>
#include <cstddef>
#include <version>

#define CCL_DECL [[nodiscard]] constexpr

// NOLINTBEGIN

#define CCL_PERFECT_FORWARDING(Repr, Type)                                                         \
    template<typename Repr = Type>                                                                 \
    requires std::is_convertible_v<Repr, Type>

#define CCL_PERFECT_FORWARDING_2(Repr1, Type1, Repr2, Type2)                                       \
    template<typename Repr1 = Type1, typename Repr2 = Type2>                                       \
    requires std::is_convertible_v<Repr1, Type1> && std::is_convertible_v<Repr2, Type2>

#define CCL_PERFECT_FORWARDING_3(Repr1, Type1, Repr2, Type2, Repr3, Type3)                         \
    template<typename Repr1 = Type1, typename Repr2 = Type2, typename Repr3 = Type3>               \
    requires std::is_convertible_v<Repr1, Type1> && std::is_convertible_v<Repr2, Type2> &&         \
        std::is_convertible_v<Repr3, Type3>

// NOLINTEND

#define CCL_STRX(x) #x
#define CCL_STR(x) CCL_STRX(x)
#define CCL_JOIN_STR(x, y) CCL_STR(x##y)
#define CCL_FORCE_EXPAND(x, y) CCL_CONCATENATE(x, y)
#define CCL_CONCATENATE(x, y) x##y

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
#    define CCL_RUNTIME_BRANCH not consteval
#else
#    define CCL_RUNTIME_BRANCH (not std::is_constant_evaluated())
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

#endif /* CCL_PROJECT_DEFINES_HPP */
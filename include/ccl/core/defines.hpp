#ifndef CCL_PROJECT_DEFINES_HPP
#define CCL_PROJECT_DEFINES_HPP

#include <cassert>
#include <cstddef>
#include <version>

#define CCL_DECL [[nodiscard]] constexpr

#define CCL_CONCATENATE(x, y) x##y
#define CCL_FORCE_EXPAND(x, y) CCL_CONCATENATE(x, y)
#define CCL_UNIQUE_IDENT CCL_FORCE_EXPAND(_cerblib_uid, __COUNTER__)

#define CCL_PERFECT_FORWARDING(Repr, Type)                                                         \
    template<typename Repr = Type>                                                                 \
    requires std::is_convertible_v<Repr, Type>

#define CCL_PERFECT_FORWARDING_2(Repr1, Repr2, Type1, Type2)                                       \
    template<typename Repr1 = Type1, typename Repr2 = Type2>                                       \
    requires std::is_convertible_v<Repr1, Type1> && std::is_convertible_v<Repr2, Type2>

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

#define CCL_HAS_CONSTEXPR_STRING                                                                   \
    __cpp_lib_constexpr_string >= 201907L && (!__clang__ || __clang_major__ >= 15)

#define CCL_HAS_CONSTEXPR_VECTOR                                                                   \
    __cpp_lib_constexpr_vector >= 201907L && (!__clang__ || __clang_major__ >= 15)

// NOLINTBEGIN
#define CCL_DERIVED_CONSTRUCTORS(Class)                                                            \
    Class() = default;                                                                             \
    Class(const Class &) = default;                                                                \
    Class(Class &&) noexcept = default;                                                            \
    ~Class() = default;                                                                            \
    auto operator=(const Class &)->Class & = default;                                              \
    auto operator=(Class &&) noexcept->Class & = default
// NOLINTEND

#endif /* CCL_PROJECT_DEFINES_HPP */

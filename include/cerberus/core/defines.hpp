#ifndef CERBERUS_PROJECT_DEFINES_HPP
#define CERBERUS_PROJECT_DEFINES_HPP

#include <cstddef>
#include <version>

#define CERBLIB_DECL [[nodiscard]] constexpr

#define CERBLIB_CONCATENATE(x, y) x##y
#define CERBLIB_FORCE_EXPAND(x, y) CERBLIB_CONCATENATE(x, y)
#define CERBLIB_UNIQUE_IDENT CERBLIB_FORCE_EXPAND(_cerblib_uid, __COUNTER__)

#define CERBLIB_PERFECT_FORWARDING(Repr, Type)                                                     \
    template<typename Repr = Type>                                                                 \
        requires std::is_convertible_v<Repr, Type>

#define CERBLIB_PERFECT_FORWARDING_2(Repr1, Repr2, Type1, Type2)                                   \
    template<typename Repr1 = Type1, typename Repr2 = Type2>                                       \
        requires std::is_convertible_v<Repr1, Type1> && std::is_convertible_v<Repr2, Type2>

#if INTPTR_MAX == INT32_MAX
#    define CERBLIB_64BIT false
#else
#    define CERBLIB_64BIT true
#endif /* CERBLIB_64BIT */

#if __cpp_if_consteval
#    define CERBLIB_COMPILE_TIME_BRANCH consteval
#else
#    define CERBLIB_COMPILE_TIME_BRANCH (std::is_constant_evaluated())
#endif /* CERBLIB_COMPILE_TIME_BRANCH */

#if __cpp_if_consteval
#    define CERBLIB_RUNTIME_BRANCH not consteval
#else
#    define CERBLIB_RUNTIME_BRANCH (not std::is_constant_evaluated())
#endif /* CERBLIB_RUNTIME_BRANCH */

#if defined(__clang__)
#    define CERBLIB_TRIVIAL_ABI [[clang::trivial_abi]]
#else
#    define CERBLIB_TRIVIAL_ABI
#endif /* CERBLIB_TRIVIAL_ABI */

#define CERBLIB_HAS_CONSTEXPR_STRING                                                               \
    __cpp_lib_constexpr_string >= 201907L && (!__clang__ || __clang_major__ >= 15)

#define CERBLIB_HAS_CONSTEXPR_VECTOR                                                               \
    __cpp_lib_constexpr_vector >= 201907L && (!__clang__ || __clang_major__ >= 15)

// NOLINTBEGIN
#define CERBLIB_DERIVED_CONSTRUCTORS(Class)                                                        \
    Class() = default;                                                                             \
    Class(const Class &) = default;                                                                \
    Class(Class &&) noexcept = default;                                                            \
    ~Class() = default;                                                                            \
    auto operator=(const Class &)->Class & = default;                                              \
    auto operator=(Class &&) noexcept->Class & = default
// NOLINTEND

#endif /* CERBERUS_PROJECT_DEFINES_HPP */

#ifndef CERBERUS_PROJECT_DEFINES_HPP
#define CERBERUS_PROJECT_DEFINES_HPP

#include <cstddef>
#include <version>

#define CERBLIB_DECL [[nodiscard]] constexpr

#define CERBLIB_CONCATENATE(x, y) x##y
#define CERBLIB_FORCE_EXPAND(x, y) CERBLIB_CONCATENATE(x, y)
#define CERBLIB_UNIQUE_IDENT CERBLIB_FORCE_EXPAND(_cerblib_uid, __COUNTER__)

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

#endif /* CERBERUS_PROJECT_DEFINES_HPP */

#ifndef CERBERUS_PROJECT_DEBUG_HPP
#define CERBERUS_PROJECT_DEBUG_HPP

#include <cerberus/comparison.hpp>
#include <cerberus/core/defines.hpp>
#include <fmt/format.h>
#include <source_location>

#define UNUSED_DECL [[maybe_unused]] const auto CERBLIB_UNIQUE_IDENT =
#define RUNTIME_TEST [[maybe_unused]] static bool CERBLIB_UNIQUE_IDENT = []() -> bool
#define CONSTEXPR_TEST [[maybe_unused]] constexpr static bool CERBLIB_UNIQUE_IDENT = []() -> bool

#if defined(CERBLIB_HAS_CONSTEXPR_VECTOR) &&                                                       \
    !defined(_MSC_VER)// MSVC has some stupid errors with constexpr string
#    define VECTOR_TEST CONSTEXPR_TEST
#else
#    define VECTOR_TEST RUNTIME_TEST
#endif

#if defined(CERBLIB_HAS_CONSTEXPR_STRING) &&                                                       \
    !defined(_MSC_VER)// MSVC has some stupid errors with constexpr string
#    define STRING_TEST CONSTEXPR_TEST
#else
#    define STRING_TEST RUNTIME_TEST
#endif

#define ERROR_EXPECTED(expression_with_error, error_type, error_message)                           \
    try {                                                                                          \
        expression_with_error;                                                                     \
        cerb::debug::assertTrue(false);                                                            \
    } catch (error_type const &error) {                                                            \
        cerb::debug::assertEqual(std::string_view(error.what()), error_message);                   \
    }

namespace cerb::debug
{
    template<typename T>
    concept Printable = std::is_constructible_v<fmt::formatter<T>>;

    [[nodiscard]] inline auto convertLocation(std::source_location location) -> std::string
    {
        return ::fmt::format(
            "{}, function: {}, line: {}", location.file_name(), location.function_name(),
            location.line());
    }

    constexpr auto
        assertTrue(bool value, std::source_location location = std::source_location::current())
            -> void
    {
        if (!value) {
            throw std::runtime_error(
                ::fmt::format("Expected true, got false in {}", convertLocation(location)));
        }
    }

    constexpr auto
        assertFalse(bool value, std::source_location location = std::source_location::current())
            -> void
    {
        if (value) {
            throw std::runtime_error(
                ::fmt::format("Expected false, got true in {}", convertLocation(location)));
        }
    }

    template<typename T, typename U>
    constexpr auto assertEqual(
        const T &lhs, const U &rhs, std::source_location location = std::source_location::current())
        -> void
    {
        if (safeNotEqual(lhs, rhs)) {
            if constexpr (Printable<T>) {
                throw std::runtime_error(::fmt::format(
                    "Expected {}, got {} in {}", rhs, lhs, convertLocation(location)));
            } else {
                throw std::runtime_error(::fmt::format("Error in {}", convertLocation(location)));
            }
        }
    }

    template<typename T, typename U>
    constexpr auto assertNotEqual(
        const T &lhs, const U &rhs, std::source_location location = std::source_location::current())
        -> void
    {
        if (safeEqual(lhs, rhs)) {
            if constexpr (Printable<T>) {
                throw std::runtime_error(::fmt::format(
                    "Expected {} is not equal to {} in {}", lhs, rhs, convertLocation(location)));
            } else {
                throw std::runtime_error(::fmt::format("Error in {}", convertLocation(location)));
            }
        }
    }
}// namespace cerb::debug

#endif /* CERBERUS_PROJECT_DEBUG_HPP */

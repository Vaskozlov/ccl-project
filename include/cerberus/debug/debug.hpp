#ifndef CERBERUS_PROJECT_DEBUG_HPP
#define CERBERUS_PROJECT_DEBUG_HPP

#include <cerberus/comparison.hpp>
#include <cerberus/core/defines.hpp>
#include <cerberus/debug/debug_location.hpp>
#include <fmt/format.h>

#define STATIC_VARIABLE [[maybe_unused]] static constexpr
#define UNUSED_DECL [[maybe_unused]] auto CERBLIB_UNIQUE_IDENT =

#define RUNTIME_TEST [[maybe_unused]] static bool CERBLIB_UNIQUE_IDENT = []() -> bool
#define CONSTEXPR_TEST STATIC_VARIABLE bool CERBLIB_UNIQUE_IDENT = []() -> bool

#define ASSERT_TRUE(value) cerb::debug::assertTrue(value, CREATE_DEBUG_LOCATION)
#define ASSERT_FALSE(value) cerb::debug::assertFalse(value, CREATE_DEBUG_LOCATION)
#define ASSERT_EQ(lhs, rhs) cerb::debug::assertEqual(lhs, rhs, CREATE_DEBUG_LOCATION)
#define ASSERT_NOT_EQ(lhs, rhs) cerb::debug::assertNotEqual(lhs, rhs, CREATE_DEBUG_LOCATION)

#define STATIC_ASSERT_EQ(lhs, rhs) static_assert(cerb::safeEqual(lhs, rhs))
#define STATIC_ASSERT_NOT_EQ(lhs, rhs) static_assert(cerb::safeNotEqual(lhs, rhs))

#ifdef CERBLIB_HAS_CONSTEXPR_VECTOR
#    define VECTOR_TEST CONSTEXPR_TEST
#    define VECTOR_ASSERT_TRUE(value) static_assert(value)
#    define VECTOR_ASSERT_FALSE(value) static_assert(!value)
#    define VECTOR_ASSERT_EQ(lhs, rhs) STATIC_ASSERT_EQ(lhs, rhs)
#    define VECTOR_ASSERT_NOT_EQ(lhs, rhs) STATIC_ASSERT_NOT_EQ(lhs, rhs)
#else
#    define VECTOR_TEST RUNTIME_TEST
#    define VECTOR_ASSERT_TRUE(value) ASSERT_TRUE(value)
#    define VECTOR_ASSERT_FALSE(value) ASSERT_FALSE(!value)
#    define VECTOR_ASSERT_EQ(lhs, rhs) ASSERT_EQ(lhs, rhs)
#    define VECTOR_ASSERT_NOT_EQ(lhs, rhs) ASSERT_NOT_EQ(lhs, rhs)
#endif

#ifdef CERBLIB_HAS_CONSTEXPR_STRING
#    define STRING_TEST CONSTEXPR_TEST
#    define STRING_ASSERT_TRUE(value) static_assert(value)
#    define STRING_ASSERT_FALSE(value) static_assert(!value)
#    define STRING_ASSERT_EQ(lhs, rhs) STATIC_ASSERT_EQ(lhs, rhs)
#    define STRING_ASSERT_NOT_EQ(lhs, rhs) STATIC_ASSERT_NOT_EQ(lhs, rhs)
#else
#    define STRING_TEST RUNTIME_TEST
#    define STRING_ASSERT_TRUE(value) ASSERT_TRUE(value)
#    define STRING_ASSERT_FALSE(value) ASSERT_FALSE(!value)
#    define STRING_ASSERT_EQ(lhs, rhs) ASSERT_EQ(lhs, rhs)
#    define STRING_ASSERT_NOT_EQ(lhs, rhs) ASSERT_NOT_EQ(lhs, rhs)
#endif

#define ERROR_EXPECTED(expression_with_error, error_type, error_message)                           \
    try {                                                                                          \
        expression_with_error;                                                                     \
        ASSERT_TRUE(false);                                                                        \
    } catch (error_type const &error) {                                                            \
        ASSERT_EQ(std::string_view(error.what()), error_message);                                  \
    }

namespace cerb::debug
{
    template<typename T>
    concept Printable = std::is_constructible_v<fmt::formatter<T>>;

    constexpr auto assertTrue(bool value, Location location) -> void
    {
        if (!value) {
            throw std::runtime_error(
                ::fmt::format("Expected true, got false in {}", location.toStr()));
        }
    }

    constexpr auto assertFalse(bool value, Location location) -> void
    {
        if (value) {
            throw std::runtime_error(
                ::fmt::format("Expected false, got true in {}", location.toStr()));
        }
    }

    template<typename T, typename U>
    constexpr auto assertEqual(const T &lhs, const U &rhs, Location location) -> void
    {
        if (safeNotEqual<T>(lhs, rhs)) {
            if constexpr (Printable<T>) {
                throw std::runtime_error(
                    ::fmt::format("Expected {}, got {} in {}", lhs, rhs, location.toStr()));
            } else {
                throw std::runtime_error(::fmt::format("Error in {}", location.toStr()));
            }
        }
    }

    template<typename T, typename U>
    constexpr auto assertNotEqual(const T &lhs, const U &rhs, Location location) -> void
    {
        if (safeEqual<T>(lhs, rhs)) {
            if constexpr (Printable<T>) {
                throw std::runtime_error(::fmt::format(
                    "Expected {} is not equal to {} in {}", lhs, rhs, location.toStr()));
            } else {
                throw std::runtime_error(::fmt::format("Error in {}", location.toStr()));
            }
        }
    }
}// namespace cerb::debug

#endif /* CERBERUS_PROJECT_DEBUG_HPP */

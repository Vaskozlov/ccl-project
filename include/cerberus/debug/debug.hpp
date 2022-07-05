#ifndef CERBERUS_PROJECT_DEBUG_HPP
#define CERBERUS_PROJECT_DEBUG_HPP

#include <cerberus/comparison.hpp>
#include <cerberus/core/defines.hpp>
#include <cerberus/debug/debug_location.hpp>
#include <fmt/format.h>

#define STATIC_VARIABLE [[maybe_unused]] static constexpr
#define UNUSED_DECL [[maybe_unused]] auto CERBLIB_UNIQUE_IDENT =

#define DEBUG_SCOPE CREATE_DEBUG_SCOPE(CERBLIB_UNIQUE_IDENT)

#define ASSERT_TRUE(value) cerb::debug::assertTrue(value, CREATE_DEBUG_LOCATION)
#define ASSERT_FALSE(value) cerb::debug::assertFalse(value, CREATE_DEBUG_LOCATION)
#define ASSERT_EQ(lhs, rhs) cerb::debug::assertEqual(lhs, rhs, CREATE_DEBUG_LOCATION)
#define ASSERT_NOT_EQ(lhs, rhs) cerb::debug::assertNotEqual(lhs, rhs, CREATE_DEBUG_LOCATION)

#define STATIC_ASSERT_EQ(lhs, rhs) static_assert(cerb::safeEqual(lhs, rhs))
#define STATIC_ASSERT_NOT_EQ(lhs, rhs) static_assert(cerb::safeNotEqual(lhs, rhs))

#define ERROR_EXPECTED(expression_with_error, error_type, error_message)                           \
    try {                                                                                          \
        expression_with_error;                                                                     \
        ASSERT_TRUE(false);                                                                        \
    } catch (error_type const &error) {                                                            \
        ASSERT_EQ(std::string_view(error.what()), error_message);                                  \
    }

#define CREATE_DEBUG_SCOPE(name)                                                                   \
    [[maybe_unused]] static auto name()->bool;                                                     \
    static bool CERBLIB_UNIQUE_IDENT = name();                                                     \
    [[maybe_unused]] static auto name()->bool


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

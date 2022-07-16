#include <cerberus/debug/debug.hpp>
#include <cerberus/string_map.hpp>

using namespace cerb::debug;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::StringMap<char, int>{ { "Hello!", 1 } };

    auto [success, value, repr] = string_map.matches("Hello, World!");

    assertFalse(success);

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::StringMap<char, int>{ { "Hello!", 1 } };

    auto [success, value, repr] = string_map.matches("Hell");

    assertFalse(success);

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::StringMap<char, int>{ { "Hello", 1 } };

    auto [success, value, repr] = string_map.matches("Hello, World!");

    assertTrue(success);
    assertEqual(value, 1);
    assertEqual(repr, "Hello");

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::StringMap<char, int>{};
    string_map.addString("test", 2);

    auto [success, value, repr] = string_map.matches("testtest");

    assertTrue(success);
    assertEqual(value, 2);
    assertEqual(repr, "test");

    return {};
}
();

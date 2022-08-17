#include <cerberus/debug/debug_file.hpp>
#include <cerberus/string_map.hpp>

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::StringMap<char, int>{ { "Hello!", 1 } };
    auto match_result = string_map.match("Hello, World!");

    assertFalse(match_result.has_value());

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::StringMap<char, int>{ { "Hello!", 1 } };
    auto match_result = string_map.match("Hell");

    assertFalse(match_result.has_value());

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::StringMap<char, int>{ { "Hello", 1 } };
    auto match_result = string_map.match("Hello, World!");

    assertTrue(match_result.has_value());

    auto &[matching_str, value] = *match_result;

    assertEqual(value, 1);
    assertEqual(matching_str, "Hello");

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::StringMap<char, int>{};
    string_map.addString("test", 2);

    auto match_result = string_map.match("testtest");

    assertTrue(match_result.has_value());

    auto &[matching_str, value] = *match_result;

    assertEqual(value, 2);
    assertEqual(matching_str, "test");

    return {};
}
();

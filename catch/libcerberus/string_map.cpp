#include <cerberus/debug/debug_file.hpp>
#include <cerberus/string_map.hpp>

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::StringMap<int>{ { u8"Hello!", 1 } };
    auto match_result = string_map.match(u8"Hello, World!");

    assertFalse(match_result.has_value());

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::StringMap<int>{ { u8"Hello!", 1 } };
    auto match_result = string_map.match(u8"Hell");

    assertFalse(match_result.has_value());

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::StringMap<int>{ { u8"Hello", 1 } };
    auto match_result = string_map.match(u8"Hello, World!");

    assertTrue(match_result.has_value());

    auto &[matching_str, value] = *match_result;

    assertEqual(value, 1);
    assertEqual(matching_str, u8"Hello");

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::StringMap<int>{};
    string_map.addString(u8"test", 2);

    auto match_result = string_map.match(u8"testtest");

    assertTrue(match_result.has_value());

    auto &[matching_str, value] = *match_result;

    assertEqual(value, 2);
    assertEqual(matching_str, u8"test");

    return {};
}
();

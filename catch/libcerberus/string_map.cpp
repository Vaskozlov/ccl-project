#include <cerberus/debug/debug_file.hpp>
#include <cerberus/raw_string_matcher.hpp>

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::RawStringMatcher{ { u8"Hello!", 1 } };
    auto match_result = string_map.match(u8"Hello, World!");

    assertFalse(match_result.has_value());

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::RawStringMatcher{ { u8"Hello!", 1 } };
    auto match_result = string_map.match(u8"Hell");

    assertFalse(match_result.has_value());

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::RawStringMatcher{ { u8"Hello", 1 } };
    auto match_result = string_map.match(u8"Hello, World!");

    assertTrue(match_result.has_value());

    auto &[matching_str, value] = *match_result;

    assertEqual(value, 1_ZU);
    assertEqual(matching_str, u8"Hello");

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto string_map = cerb::RawStringMatcher{};
    string_map.addString(u8"test", 2);

    auto match_result = string_map.match(u8"testtest");

    assertTrue(match_result.has_value());

    auto &[matching_str, value] = *match_result;

    assertEqual(value, 2_ZU);
    assertEqual(matching_str, u8"test");

    return {};
}
();

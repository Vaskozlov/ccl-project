#include <ccl/debug/debug_file.hpp>
#include <ccl/raw_string_matcher.hpp>

using namespace ccl;

BOOST_AUTO_TEST_CASE(StringMapMismatch)
{
    auto string_map = RawStringMatcher{ { u8"Hello!", 1 } };
    auto match_result = string_map.match(u8"Hello, World!");

    BOOST_ASSERT(not match_result.has_value());
}

BOOST_AUTO_TEST_CASE(StringMapMatch)
{
    auto string_map = RawStringMatcher{ { u8"Hello", 1 } };
    auto match_result = string_map.match(u8"Hello, World!");

    BOOST_ASSERT(match_result.has_value());

    DEBUG_VAR & [ matching_str, value ] = *match_result;

    BOOST_ASSERT(value == 1);
    BOOST_ASSERT(matching_str == u8"Hello");
}

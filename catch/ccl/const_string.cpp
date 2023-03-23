#include <ccl/const_string.hpp>
#include <ccl/debug/debug_file.hpp>

using namespace ccl;

template<ConstString String>
auto testConstString(string_view input) -> bool
{
    DEBUG_VAR test_string = input;

    REQUIRE(String.empty() == test_string.empty());
    REQUIRE(String.size() == test_string.size());
    REQUIRE(as<string_view>(String) == test_string);

    REQUIRE(std::ranges::equal(String, test_string));
    REQUIRE(std::equal(String.begin(), String.end(), test_string.begin()));

    return true;
}

TEST_CASE("ConstStringEmptyInput", "[ConstString]")
{
    REQUIRE(testConstString<"">(""));
}

TEST_CASE("ConstStringSingleBasicInput", "[ConstString]")
{
    REQUIRE(testConstString<"Hello, World!">("Hello, World!"));
}

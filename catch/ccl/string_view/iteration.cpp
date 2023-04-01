#include <ccl/debug/debug_file.hpp>
#include <ccl/string_view.hpp>

using namespace ccl;
using namespace string_view_literals;
using namespace std::string_view_literals;

TEST_CASE("StringViewIteration", "[StringView]")
{
    DEBUG_VAR std_string = "Hello, World!"sv;
    DEBUG_VAR ccl_string = "Hello, World!"_sv;

    REQUIRE(std::equal(std_string.cbegin(), std_string.cend(), ccl_string.cbegin(), ccl_string.cend()));
    REQUIRE(std::equal(std_string.begin(), std_string.end(), ccl_string.begin()));
}

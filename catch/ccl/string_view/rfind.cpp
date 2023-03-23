#include <ccl/debug/debug_file.hpp>
#include <ccl/string_view.hpp>

using namespace ccl::string_view_literals;
using namespace std::string_view_literals;

TEST_CASE("StringViewRfind", "[StringView]")
{
    DEBUG_VAR std_string = "Hello, World!"sv;
    DEBUG_VAR ccl_string = "Hello, World!"_sv;

    REQUIRE(ccl_string.rfind('H').value() == std_string.rfind('H'));
    REQUIRE(ccl_string.rfind(',').value() == std_string.rfind(','));
    REQUIRE(ccl_string.rfind('!').value() == std_string.rfind('!'));
    REQUIRE(!ccl_string.rfind('A').has_value());
}

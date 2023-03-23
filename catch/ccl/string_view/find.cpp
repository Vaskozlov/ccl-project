#include <ccl/debug/debug_file.hpp>
#include <ccl/string_view.hpp>

using namespace ccl;
using namespace string_view_literals;
using namespace std::string_view_literals;

TEST_CASE("StringViewFind", "[StringView]")
{
    DEBUG_VAR std_string = "Hello, World!"sv;
    DEBUG_VAR cerb_string = "Hello, World!"_sv;

    REQUIRE(cerb_string.find('H').value() == std_string.find('H'));
    REQUIRE(cerb_string.find(',').value() == std_string.find(','));
    REQUIRE(cerb_string.find('!').value() == std_string.find('!'));

    REQUIRE(!cerb_string.find('A').has_value());

    REQUIRE(cerb_string.find('l', 6).value() == std_string.find('l', 6));
    REQUIRE(cerb_string.find('l', cerb_string.begin() + 6).value() == std_string.find('l', 6));
}

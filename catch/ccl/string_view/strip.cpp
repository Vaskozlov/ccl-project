#include <ccl/debug/debug_file.hpp>
#include <ccl/string_view.hpp>

using namespace ccl;
using namespace string_view_literals;
using namespace std::string_view_literals;

TEST_CASE("StringViewLeftStrip", "[StringView]")
{
    DEBUG_VAR own_string = "115223311"_sv;
    own_string.leftStrip("15");
    REQUIRE(own_string == "223311"_sv);
}

TEST_CASE("StringViewRightStrip", "[StringView]")
{
    DEBUG_VAR own_string = "1152233611"_sv;
    own_string.rightStrip("16");
    REQUIRE(own_string == "1152233"_sv);
}

TEST_CASE("StringViewStrip", "[StringView]")
{
    DEBUG_VAR own_string = "1152233611"_sv;
    own_string.strip("156");
    REQUIRE(own_string == "2233"_sv);
}


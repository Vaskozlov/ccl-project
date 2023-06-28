#include <ccl/debug/debug_file.hpp>
#include <ccl/string_view.hpp>

using namespace ccl;
using namespace string_view_literals;
using namespace std::string_view_literals;

TEST_CASE("StringViewLeftStrip", "[StringView]")
{
    DEBUG_VAR cerb_string = "115223311"_sv;
    cerb_string.leftStrip("15");
    REQUIRE(cerb_string == "223311"_sv);
}

TEST_CASE("StringViewRightStrip", "[StringView]")
{
    DEBUG_VAR cerb_string = "1152233611"_sv;
    cerb_string.rightStrip("16");
    REQUIRE(cerb_string == "1152233"_sv);
}

TEST_CASE("StringViewStrip", "[StringView]")
{
    DEBUG_VAR cerb_string = "1152233611"_sv;
    cerb_string.strip("156");
    REQUIRE(cerb_string == "2233"_sv);
}


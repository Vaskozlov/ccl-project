#include <ccl/core/operations.hpp>
#include <ccl/debug/debug_file.hpp>

using namespace ccl;

TEST_CASE("LogicalOr", "[LogicalOps]")
{
    REQUIRE(lor(true));
    REQUIRE(!lor(false));

    REQUIRE(lor(true, true));
    REQUIRE(lor(true, false));
    REQUIRE(!lor(false, false));

    REQUIRE(lor(true, true, true));
    REQUIRE(lor(true, true, false));
    REQUIRE(!lor(false, false, false));
}

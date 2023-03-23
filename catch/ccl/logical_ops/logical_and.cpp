#include <ccl/core/operations.hpp>
#include <ccl/debug/debug_file.hpp>

using namespace ccl;

TEST_CASE("LogicalAnd", "[LogicalOps]")
{
    REQUIRE(land(true));
    REQUIRE(!land(false));

    REQUIRE(land(true, true));
    REQUIRE(!land(true, false));

    REQUIRE(land(true, true, true));
    REQUIRE(!land(true, true, false));
    REQUIRE(!land(false, false, false));
}

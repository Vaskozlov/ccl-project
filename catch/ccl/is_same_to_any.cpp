#include <ccl/core/concepts.hpp>
#include <ccl/debug/debug_file.hpp>

using namespace ccl;

TEST_CASE("SameToAny", "[Concepts]")
{
    REQUIRE(IsSameToAny<int, float, double, int>);
    REQUIRE(!IsSameToAny<int, float, double, unsigned int>);
}

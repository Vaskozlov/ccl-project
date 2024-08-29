#include <ccl/debug/debug.hpp>
#include <ccl/text/location.hpp>

TEST_CASE("LocationTracking", "[Location]")
{
    using namespace ccl;

    auto location = text::Location{};

    location.next(U'a');

    REQUIRE(location.line == 1U);
    REQUIRE(location.column == 1U);

    location.next(U'\n');

    REQUIRE(location.line == 2U);
    REQUIRE(location.column == 0U);

    location.next(U'a');

    REQUIRE(location.line == 2U);
    REQUIRE(location.column == 1U);
}

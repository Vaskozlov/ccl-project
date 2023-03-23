#include <ccl/debug/debug_file.hpp>
#include <ccl/raii.hpp>

using namespace ccl;

TEST_CASE("RaiiTest")
{
    // NOLINTBEGIN

    DEBUG_VAR test_variable = 10;
    {
        const auto raii = Raii{
            [&test_variable]() {
                test_variable = 42;
            },
            [&test_variable]() {
                test_variable = 10;
            }};

        REQUIRE(test_variable == 42);
    }

    // NOLINTEND

    REQUIRE(test_variable == 10);
}

#include <ccl/debug/debug_file.hpp>
#include <ccl/raii.hpp>

using namespace ccl;

BOOST_AUTO_TEST_CASE(RaiiTest)
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

        BOOST_CHECK_EQUAL(test_variable, 42);
    }

    // NOLINTEND

    BOOST_CHECK_EQUAL(test_variable, 10);
}

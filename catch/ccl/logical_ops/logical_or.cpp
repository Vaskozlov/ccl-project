#include <ccl/core/operations.hpp>
#include <ccl/debug/debug_file.hpp>

using namespace ccl;

BOOST_AUTO_TEST_CASE(LogicalOr)
{
    BOOST_STATIC_ASSERT(lor(true));
    BOOST_STATIC_ASSERT(not lor(false));

    BOOST_STATIC_ASSERT(lor(true, true));
    BOOST_STATIC_ASSERT(lor(true, false));
    BOOST_STATIC_ASSERT(not lor(false, false));

    BOOST_STATIC_ASSERT(lor(true, true, true));
    BOOST_STATIC_ASSERT(lor(true, true, false));
    BOOST_STATIC_ASSERT(not lor(false, false, false));
}

#include <ccl/core/operations.hpp>
#include <ccl/debug/debug_file.hpp>

using namespace ccl;

BOOST_AUTO_TEST_CASE(LogicalAnd)
{
    BOOST_STATIC_ASSERT(land(true));
    BOOST_STATIC_ASSERT(!land(false));

    BOOST_STATIC_ASSERT(land(true, true));
    BOOST_STATIC_ASSERT(!land(true, false));

    BOOST_STATIC_ASSERT(land(true, true, true));
    BOOST_STATIC_ASSERT(!land(true, true, false));
    BOOST_STATIC_ASSERT(!land(false, false, false));
}

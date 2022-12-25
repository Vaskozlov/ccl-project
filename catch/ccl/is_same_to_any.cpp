#include <ccl/core/concepts.hpp>
#include <ccl/debug/debug_file.hpp>

using namespace ccl;

BOOST_AUTO_TEST_CASE(SameToAny)
{
    BOOST_STATIC_ASSERT(IsSameToAny<int, float, double, int>);
    BOOST_STATIC_ASSERT(!IsSameToAny<int, float, double, unsigned int>);
}

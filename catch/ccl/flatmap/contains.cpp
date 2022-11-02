#include <ccl/debug/debug_file.hpp>
#include <ccl/flatmap.hpp>

using namespace ccl;

BOOST_AUTO_TEST_CASE(FlatmapContains)
{
    static constexpr auto flatmap =
        StaticFlatmap<int, int, 10>{ { 10, 20 }, { 20, 30 }, { 30, 40 } };// NOLINT

    BOOST_CHECK_EQUAL(flatmap.size(), 3);
    BOOST_CHECK_EQUAL(flatmap.capacity(), 10);

    BOOST_ASSERT(flatmap.contains(10));
    BOOST_ASSERT(flatmap.contains(20));
    BOOST_ASSERT(flatmap.contains(30));

    BOOST_ASSERT(not flatmap.contains(0));
    BOOST_ASSERT(not flatmap.contains(40));
}

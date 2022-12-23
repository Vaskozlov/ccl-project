#include <ccl/debug/debug_file.hpp>
#include <ccl/flatmap.hpp>

using namespace ccl;
using namespace std::string_view_literals;

BOOST_AUTO_TEST_CASE(FlatmapIndexing)
{
    static constexpr auto flatmap = StaticFlatmap<int, int, 10>{{10, 20}, {20, 30}, {30, 40}};

    BOOST_CHECK_EQUAL(flatmap[10], 20);
    BOOST_CHECK_EQUAL(flatmap[20], 30);
    BOOST_CHECK_EQUAL(flatmap[30], 40);

    BOOST_CHECK_EXCEPTION(
        DEBUG_VAR _ = flatmap[40], std::out_of_range,
        [](const std::out_of_range &exception) { return "key not found"sv == exception.what(); });
}

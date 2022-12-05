#include <ccl/debug/debug_file.hpp>
#include <ccl/flatmap.hpp>

using namespace ccl;
using namespace std::string_view_literals;

// NOLINTBEGIN

BOOST_AUTO_TEST_CASE(FlatmapOverflow)
{
    auto flatmap = StaticFlatmap<int, int, 10>{{10, 20}, {20, 30}, {30, 40}, {30, 40},  {40, 50},
                                               {60, 70}, {70, 80}, {80, 80}, {90, 100}, {100, 110}};

    BOOST_CHECK_EQUAL(flatmap.size(), flatmap.capacity());

    BOOST_CHECK_EXCEPTION(
        flatmap.insert(110, 120), std::out_of_range,
        [](const std::out_of_range &exception) { return "flatmap is full"sv == exception.what(); });
}

// NOLINTEND

#include <cerberus/debug/debug_file.hpp>
#include <cerberus/flatmap.hpp>

using namespace std::string_view_literals;

// NOLINTBEGIN

BOOST_AUTO_TEST_CASE(FlatmapOverflow)
{
    auto flatmap = cerb::Flatmap<int, int, 10>{ { 10, 20 },  { 20, 30 },  { 30, 40 }, { 30, 40 },
                                                { 40, 50 },  { 60, 70 },  { 70, 80 }, { 80, 80 },
                                                { 90, 100 }, { 100, 110 } };

    BOOST_CHECK_EQUAL(flatmap.size(), flatmap.capacity());

    BOOST_CHECK_EXCEPTION(
        flatmap.insert(110, 120), cerb::OutOfRange,
        [](const cerb::OutOfRange &exception) { return "flatmap is full"sv == exception.what(); });
}

// NOLINTEND

#include <cerberus/debug/debug_file.hpp>
#include <cerberus/flatmap.hpp>

using namespace std::string_view_literals;

BOOST_AUTO_TEST_CASE(FlatmapIndexing)
{
    static constexpr auto flatmap =
        cerb::Flatmap<int, int, 10>{ { 10, 20 }, { 20, 30 }, { 30, 40 } };

    BOOST_CHECK_EQUAL(flatmap[10], 20);
    BOOST_CHECK_EQUAL(flatmap[20], 30);
    BOOST_CHECK_EQUAL(flatmap[30], 40);

    BOOST_CHECK_EXCEPTION(
        DEBUG_DECL _ = flatmap.at(40), cerb::KeyNotFound,
        [](const cerb::KeyNotFound &exception) { return "key not found"sv == exception.what(); });
}

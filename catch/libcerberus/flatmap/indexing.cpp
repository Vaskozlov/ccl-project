#include <boost/test/unit_test.hpp>
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
        [[maybe_unused]] auto _ = flatmap.at(40), cerb::KeyNotFound, [](const auto &exception) {
            return "key not found"sv == exception.what();
        });
}

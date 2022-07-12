#include <cerberus/debug/debug.hpp>
#include <cerberus/flatmap.hpp>

// NOLINTBEGIN

using namespace cerb::debug;
using namespace cerb::integral_literals;

using Flatmap = cerb::Flatmap<int, int, 10>;
template class cerb::Flatmap<int, int, 10>;

CONSTEXPR_TEST
{
    auto flatmap = Flatmap{ { 10, 20 }, { 20, 30 }, { 30, 40 } };

    assertEqual(flatmap.size(), 3_ZU);
    assertEqual(flatmap.capacity(), 10_ZU);

    assertTrue(flatmap.contains(10));
    assertTrue(flatmap.contains(20));
    assertTrue(flatmap.contains(30));

    assertTrue(!flatmap.contains(0));
    assertTrue(!flatmap.contains(40));

    return {};
}
();

// NOLINTEND

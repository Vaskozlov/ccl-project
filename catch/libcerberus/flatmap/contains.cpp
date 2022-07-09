#include <cerberus/debug/debug.hpp>
#include <cerberus/flatmap.hpp>

// NOLINTBEGIN

using namespace cerb::integral_literals;

using Flatmap = cerb::Flatmap<int, int, 10>;
template class cerb::Flatmap<int, int, 10>;

CONSTEXPR_TEST
{
    auto flatmap = Flatmap{ { 10, 20 }, { 20, 30 }, { 30, 40 } };

    ASSERT_EQ(flatmap.size(), 3_ZU);
    ASSERT_EQ(flatmap.capacity(), 10_ZU);

    ASSERT_TRUE(flatmap.contains(10));
    ASSERT_TRUE(flatmap.contains(20));
    ASSERT_TRUE(flatmap.contains(30));

    ASSERT_TRUE(!flatmap.contains(0));
    ASSERT_TRUE(!flatmap.contains(40));

    return {};
}
();

// NOLINTEND

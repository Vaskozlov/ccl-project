#include <cerberus/debug/debug.hpp>
#include <cerberus/flatmap.hpp>

// NOLINTBEGIN

using Flatmap = cerb::Flatmap<int, int, 10>;

RUNTIME_TEST
{
    STATIC_VARIABLE auto flatmap = Flatmap{ { 10, 20 }, { 20, 30 }, { 30, 40 } };

    STATIC_ASSERT_EQ(flatmap[10], 20);
    STATIC_ASSERT_EQ(flatmap[20], 30);
    STATIC_ASSERT_EQ(flatmap[30], 40);

    ERROR_EXPECTED(UNUSED_DECL flatmap.at(40), cerb::KeyNotFound, "key not found");
    return {};
}
();

// NOLINTEND

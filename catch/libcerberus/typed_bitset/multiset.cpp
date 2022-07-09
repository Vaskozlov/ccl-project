#include <cerberus/debug/debug.hpp>
#include <cerberus/typed_bitset.hpp>

using TypedBitset = cerb::TypedBitset<cerb::u32>;

// NOLINTBEGIN

RUNTIME_TEST
{
    auto bitset = TypedBitset{};
    bitset.set(1, 10, true);

    ASSERT_FALSE(bitset.at(0));

    for (cerb::u32 i = 1; i <= 10; ++i) {
        ASSERT_TRUE(bitset.at(i));
    }

    ASSERT_FALSE(bitset.at(11));
    ASSERT_FALSE(bitset.at(1024));

    return {};
}
();

// NOLINTEND

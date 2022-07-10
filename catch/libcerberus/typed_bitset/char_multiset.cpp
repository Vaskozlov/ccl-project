#include <cerberus/debug/debug.hpp>
#include <cerberus/typed_bitset.hpp>

using TypedBitset = cerb::TypedBitset<char>;

// NOLINTBEGIN

RUNTIME_TEST
{
    auto bitset = TypedBitset{};
    bitset.set(1, 10, true);

    ASSERT_FALSE(bitset.at(0));

    for (char i = 1; i <= 10; ++i) {
        ASSERT_TRUE(bitset.at(i));
    }

    ASSERT_FALSE(bitset.at(11));

    return {};
}
();

// NOLINTEND

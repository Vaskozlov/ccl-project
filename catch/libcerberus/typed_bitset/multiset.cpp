#include <cerberus/debug/debug_file.hpp>
#include <cerberus/typed_bitset.hpp>

using TypedBitset = cerb::TypedBitset<cerb::u32>;

// NOLINTBEGIN

RUNTIME_TEST
{
    auto bitset = TypedBitset{};
    bitset.set(1, 10, true);

    assertFalse(bitset.at(0));

    for (cerb::u32 i = 1; i <= 10; ++i) {
        assertTrue(bitset.at(i));
    }

    assertFalse(bitset.at(11));
    assertFalse(bitset.at(1024));

    return {};
}
();

// NOLINTEND

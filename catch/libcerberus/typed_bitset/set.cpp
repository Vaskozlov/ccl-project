#include <cerberus/debug/debug_file.hpp>
#include <cerberus/typed_bitset.hpp>

using TypedBitset = cerb::TypedBitset<cerb::u32>;

// NOLINTBEGIN

RUNTIME_TEST
{
    auto bitset = TypedBitset{};
    bitset.set(0, true);

    assertTrue(bitset.at(0));
    assertFalse(bitset.at(1));
    assertFalse(bitset.at(1024));

    return {};
}
();

// NOLINTEND

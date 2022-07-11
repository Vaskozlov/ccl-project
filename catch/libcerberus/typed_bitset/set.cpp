#include <cerberus/debug/debug.hpp>
#include <cerberus/typed_bitset.hpp>

using namespace cerb::debug;
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

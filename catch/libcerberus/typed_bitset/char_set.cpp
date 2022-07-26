#include <cerberus/debug/debug_file.hpp>
#include <cerberus/typed_bitset.hpp>

using TypedBitset = cerb::TypedBitset<char>;

// NOLINTBEGIN

RUNTIME_TEST
{
    auto bitset = TypedBitset{};
    bitset.set('\0', true);

    assertTrue(bitset.at('\0'));
    assertFalse(bitset.at('\x01'));
    assertFalse(bitset.at('\xFF'));

    return {};
}
();

// NOLINTEND

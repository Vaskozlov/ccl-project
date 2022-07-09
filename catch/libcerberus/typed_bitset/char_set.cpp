#include <cerberus/debug/debug.hpp>
#include <cerberus/typed_bitset.hpp>

using TypedBitset = cerb::TypedBitset<char>;

// NOLINTBEGIN

RUNTIME_TEST
{
    auto bitset = TypedBitset{};
    bitset.set('\0', true);

    ASSERT_TRUE(bitset.at('\0'));

    ASSERT_FALSE(bitset.at('\x01'));
    ASSERT_FALSE(bitset.at('\xFF'));

    return {};
}
();

// NOLINTEND

#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/union.hpp>

using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ "[a]" };
    text_iterator.nextRawChar();

    auto union_item = Union<char>(text_iterator);
    const auto &bitset = union_item.get();

    for (char i = 0; i < 127; ++i) {// NOLINT
        if (i == 'a') {
            assertTrue(bitset.at(i));
        } else {
            assertFalse(bitset.at(i));
        }
    }

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ "[a-z_]" };
    text_iterator.nextRawChar();

    auto union_item = Union<char>(text_iterator);
    const auto &bitset = union_item.get();

    for (char i = 'a'; i <= 'z'; ++i) {
        assertTrue(bitset.at(i));
    }

    assertFalse(bitset.at('\0'));
    assertFalse(bitset.at('z' + 1));

    assertTrue(bitset.at('_'));

    return {};
}
();

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
        assertEqual(bitset.at(i), i == 'a');
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

    assertTrue(bitset.at('_'));

    assertFalse(bitset.at('\0'));
    assertFalse(bitset.at('z' + 1));

    return {};
}
();

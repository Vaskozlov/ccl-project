#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/union.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
auto shared = AnalysisShared<char>{};

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ "[a]" };
    text_iterator.nextRawChar();

    auto union_item = Union<char>(text_iterator, shared);
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

    auto union_item = Union<char>(text_iterator, shared);
    const auto &bitset = union_item.get();

    for (char i = '\0'; i != '_'; ++i) {
        assertFalse(bitset.at(i));
    }

    assertTrue(bitset.at('_'));

    for (char i = '_' + 1; i != 'a'; ++i) {
        assertFalse(bitset.at(i));
    }

    for (char i = 'a'; i <= 'z'; ++i) {
        assertTrue(bitset.at(i));
    }

    for (char i = 'z' + 1; i != '\x7F'; ++i) {
        assertFalse(bitset.at(i));
    }

    return {};
}
();

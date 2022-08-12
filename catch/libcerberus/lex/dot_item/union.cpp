#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/union.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
auto shared = AnalysisShared{};

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"[a]" };
    text_iterator.nextRawChar();

    auto union_item = Union(text_iterator, shared);
    const auto &bitset = union_item.get();

    for (char32_t i = 0; i < 127; ++i) {// NOLINT
        assertEqual(bitset.at(i), i == 'a');
    }

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"[a-z_]" };
    text_iterator.nextRawChar();

    auto union_item = Union(text_iterator, shared);
    const auto &bitset = union_item.get();

    for (char32_t i = '\0'; i != '_'; ++i) {
        assertFalse(bitset.at(i));
    }

    assertTrue(bitset.at('_'));

    for (char32_t i = '_' + 1; i != 'a'; ++i) {
        assertFalse(bitset.at(i));
    }

    for (char32_t i = 'a'; i <= 'z'; ++i) {
        assertTrue(bitset.at(i));
    }

    for (char32_t i = 'z' + 1; i != '\x7F'; ++i) {
        assertFalse(bitset.at(i));
    }

    return {};
}
();

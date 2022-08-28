#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/union.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

// NOLINTNEXTLINE
static auto shared = AnalysisShared{};

BOOST_AUTO_TEST_SUITE(ContainerUnion)

BOOST_AUTO_TEST_CASE(UnionWithOneElem)
{
    auto text_iterator = TextIterator{ u8"[a]" };
    text_iterator.next();

    auto union_item = Union(text_iterator, shared);
    DEBUG_VAR &&bitset = union_item.get();

    for (char32_t i = 0; i < 127; ++i) {// NOLINT
        BOOST_ASSERT(bitset.at(i) == (i == 'a'));
    }
}

BOOST_AUTO_TEST_CASE(UnionWithMultipleElems)
{
    auto text_iterator = TextIterator{ u8"[a-z_]" };
    text_iterator.next();

    auto union_item = Union(text_iterator, shared);
    DEBUG_VAR &&bitset = union_item.get();

    for (char32_t i = '\0'; i != '_'; ++i) {
        BOOST_ASSERT(not bitset.at(i));
    }

    BOOST_ASSERT(bitset.at('_'));

    for (char32_t i = '_' + 1; i != 'a'; ++i) {
        BOOST_ASSERT(not bitset.at(i));
    }

    for (char32_t i = 'a'; i <= 'z'; ++i) {
        BOOST_ASSERT(bitset.at(i));
    }

    for (char32_t i = 'z' + 1; i != '\x7F'; ++i) {
        BOOST_ASSERT(not bitset.at(i));
    }
}

BOOST_AUTO_TEST_SUITE_END()

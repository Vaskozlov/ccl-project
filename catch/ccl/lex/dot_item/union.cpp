#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/dot_item/union.hpp>


TEST_CASE("UnionWithOneElem", "[ContainerUnion]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{"[a]"};
    text_iterator.advance();

    auto union_item = lexer::dot_item::Union(text_iterator);
    DEBUG_VAR &&bitset = union_item.getBitset();

    for (char32_t i = 0; i < 127; ++i) {// NOLINT
        REQUIRE(bitset.at(i) == (i == 'a'));
    }
}

TEST_CASE("UnionWithMultipleElems", "[ContainerUnion]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator{"[a-z_]"};
    text_iterator.advance();

    auto union_item = lexer::dot_item::Union(text_iterator);
    DEBUG_VAR &&bitset = union_item.getBitset();

    for (char32_t i = '\0'; i != '_'; ++i) {
        REQUIRE(!bitset.at(i));
    }

    REQUIRE(bitset.at('_'));

    for (char32_t i = '_' + 1; i != 'a'; ++i) {
        REQUIRE(!bitset.at(i));
    }

    for (char32_t i = 'a'; i <= 'z'; ++i) {
        REQUIRE(bitset.at(i));
    }

    for (char32_t i = 'z' + 1; i != '\x7F'; ++i) {
        REQUIRE(!bitset.at(i));
    }
}

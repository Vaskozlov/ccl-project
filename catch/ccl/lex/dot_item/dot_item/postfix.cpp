#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/dot_item/container.hpp>

using namespace ccl;
using namespace lexer;
using namespace text;
using namespace dot_item;

TEST_CASE("TwoPostfixes", "[Container]")
{
    auto special_items = SpecialItems{};
    auto container = Container(
        TextIterator{R"([a-z]+[_]p"test"p)"},
        special_items,
        lexer::ReservedTokenMaxValue + 1,
        true);
    DEBUG_VAR &items = container.getItems();

    REQUIRE(!items[0]->hasPrefix());
    REQUIRE(!items[0]->hasPostfix());

    REQUIRE(items[1]->hasPostfix());
    REQUIRE(items[2]->hasPostfix());
}

TEST_CASE("WrongPostfixCreation", "[Container]")
{
    auto special_items = SpecialItems{};

    REQUIRE_THROWS_AS(
        Container(
            TextIterator{R"([a-z]+[_]p"test")"},
            special_items,
            lexer::ReservedTokenMaxValue + 1,
            true),
        text::TextIteratorException);
}

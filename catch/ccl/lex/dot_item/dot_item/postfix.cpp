#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/dot_item/container.hpp>

TEST_CASE("TwoPostfixes", "[Container]")
{
    using namespace ccl;

    auto special_items = lexer::SpecialItems{};
    auto container = lexer::dot_item::Container(
        text::TextIterator{R"([a-z]+[_]p"test"p)"},
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
    using namespace ccl;

    auto special_items = lexer::SpecialItems{};

    REQUIRE_THROWS_AS(
        lexer::dot_item::Container(
            text::TextIterator{R"([a-z]+[_]p"test")"},
            special_items,
            lexer::ReservedTokenMaxValue + 1,
            true),
        text::TextIteratorException);
}

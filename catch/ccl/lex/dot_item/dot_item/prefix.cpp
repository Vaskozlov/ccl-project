#include <ccl/debug/debug_file.hpp>
import ccl.lexer;
import ccl.debug;

TEST_CASE("ContainerTwoPrefixesCreation", "[Container]")
{
    using namespace ccl;

    auto special_items = lexer::dot_item::SpecialItems{};
    auto container = lexer::dot_item::Container(
        text::TextIterator{R"([a-z]*p[_]p"test")"}, special_items, debug::FirstUsableToken, true);
    DEBUG_VAR items = container.getItems();

    REQUIRE(items[0]->hasPrefix());
    REQUIRE(items[1]->hasPrefix());
    REQUIRE(!items[2]->hasPrefix());
}

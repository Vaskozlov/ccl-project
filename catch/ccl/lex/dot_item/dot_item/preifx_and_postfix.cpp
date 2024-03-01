#include <ccl/debug/debug_file.hpp>
import ccl.lexer;
import ccl.debug;

TEST_CASE("ContainerPrefixAndPostfix", "[Container]")
{
    using namespace ccl;

    auto special_items = lexer::dot_item::SpecialItems{};
    auto container = lexer::dot_item::Container(
        text::TextIterator{R"([a-z]+p[_]"test"p)"}, special_items, debug::FirstUsableToken, true);
    DEBUG_VAR items = container.getItems();

    REQUIRE(items.at(0)->hasPrefix());
    REQUIRE(!items.at(1)->hasPrefix());
    REQUIRE(!items.at(1)->hasPostfix());
    REQUIRE(items.at(2)->hasPostfix());
}

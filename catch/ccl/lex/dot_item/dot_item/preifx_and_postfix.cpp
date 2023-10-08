#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/dot_item/container.hpp>

using namespace ccl;
using namespace lexer;
using namespace text;
using namespace dot_item;

TEST_CASE("ContainerPrefixAndPostfix", "[Container]")
{
    auto special_items = SpecialItems{};
    auto container = Container(TextIterator{R"([a-z]+p[_]"test"p)"}, special_items, 2, true);
    DEBUG_VAR &&items = container.getItems();

    REQUIRE(items.at(0)->hasPrefix());
    REQUIRE(!items.at(1)->hasPrefix());
    REQUIRE(!items.at(1)->hasPostfix());
    REQUIRE(items.at(2)->hasPostfix());
}

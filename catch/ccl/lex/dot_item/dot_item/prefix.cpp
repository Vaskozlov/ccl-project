#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/dot_item/container.hpp>

using namespace ccl;
using namespace debug;
using namespace lexer;
using namespace text;
using namespace dot_item;

TEST_CASE("ContainerTwoPrefixesCreation", "[Container]")
{
    auto special_items = SpecialItems{};
    auto container =
        Container(TextIterator{R"([a-z]*p[_]p"test")"}, special_items, FirstUsableToken, true);
    DEBUG_VAR &&items = container.getItems();

    REQUIRE(items[0]->hasPrefix());
    REQUIRE(items[1]->hasPrefix());
    REQUIRE(!items[2]->hasPrefix());
}

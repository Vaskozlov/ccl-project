#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/container.hpp>

using namespace ccl;
using namespace lexer;
using namespace text;
using namespace dot_item;

TEST_CASE("SpecialItem", "[Container]")
{
    auto special_items = SpecialItems{};
    auto text_iterator = TextIterator{"+"};

    special_items.specialItems.emplace_back(TextIterator{R"(!"+")"}, special_items, 2);
    REQUIRE(special_items.checkForSpecial(text_iterator.fork()));
}

#include <ccl/debug/debug_file.hpp>
#include <ccl/lexer/dot_item/container.hpp>

using namespace ccl;
using namespace debug;
using namespace lexer;
using namespace text;
using namespace dot_item;

TEST_CASE("SpecialItem", "[Container]")
{
    auto special_items = SpecialItems{};
    const auto text_iterator = TextIterator{"+"};

    special_items.specialItems.emplace_back(
        TextIterator{R"(!"+")"}, special_items, FirstUsableToken);
    REQUIRE(special_items.checkForSpecial(text_iterator.fork()));
}

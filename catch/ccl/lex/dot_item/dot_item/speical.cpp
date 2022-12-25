#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/container.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

BOOST_AUTO_TEST_CASE(SpecialItem)
{
    auto special_items = SpecialItems{};
    auto text_iterator = TextIterator{"+"};

    special_items.special_items.emplace_back(Container(TextIterator{R"(!"+")"}, special_items, 2));
    BOOST_ASSERT(special_items.checkForSpecial(text_iterator.fork()));
}

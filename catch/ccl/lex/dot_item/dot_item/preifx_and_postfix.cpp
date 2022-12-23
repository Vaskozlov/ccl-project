#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/container.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

BOOST_AUTO_TEST_CASE(ContainerPrefixAndPostfix)
{
    auto special_items = SpecialItems{};
    auto container = Container(TextIterator{R"([a-z]+p[_]"test"p)"}, special_items, 2, true);
    DEBUG_VAR &&items = container.getItems();

    BOOST_ASSERT(items.at(0)->hasPrefix());
    BOOST_ASSERT(!items.at(1)->hasPrefix());
    BOOST_ASSERT(!items.at(1)->hasPostfix());
    BOOST_ASSERT(items.at(2)->hasPostfix());
}

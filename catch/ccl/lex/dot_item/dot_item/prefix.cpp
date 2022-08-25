#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/dot_item.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

BOOST_AUTO_TEST_CASE(DotItemTwoPrefixesCreation)
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"([a-z]*p[_]p"test")" }, 0, shared);
    DEBUG_VAR &&items = dot_item.getItems();

    BOOST_ASSERT(items[0]->hasPrefix());
    BOOST_ASSERT(items[1]->hasPrefix());
    BOOST_ASSERT(not items[2]->hasPrefix());
}

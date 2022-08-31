#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/container.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

BOOST_AUTO_TEST_CASE(ContainerTwoPrefixesCreation)
{
    auto shared = AnalysisShared{};
    auto container = Container(TextIterator{ R"([a-z]*p[_]p"test")" }, 2, shared);
    DEBUG_VAR &&items = container.getItems();

    BOOST_ASSERT(items[0]->hasPrefix());
    BOOST_ASSERT(items[1]->hasPrefix());
    BOOST_ASSERT(not items[2]->hasPrefix());
}

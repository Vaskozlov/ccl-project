#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/container.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

BOOST_AUTO_TEST_CASE(ContainerPrefixAndPostfix)
{
    auto shared = AnalysisShared{};
    auto container = Container(TextIterator{ R"([a-z]+p[_]"test"p)" }, 2, shared);
    DEBUG_VAR &&items = container.getItems();

    BOOST_ASSERT(items[0]->hasPrefix());
    BOOST_ASSERT(not items[1]->hasPrefix());
    BOOST_ASSERT(not items[1]->hasPostfix());
    BOOST_ASSERT(items[2]->hasPostfix());
}

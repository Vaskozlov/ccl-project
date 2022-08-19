#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

BOOST_AUTO_TEST_CASE(DotItemPrefixAndPostfix)
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"([a-z]+p[_]"test"p)" }, 0, shared);
    DEBUG_DECL &&items = dot_item.getItems();

    BOOST_ASSERT(items[0]->hasPrefix());
    BOOST_ASSERT(not items[1]->hasPrefix());
    BOOST_ASSERT(not items[1]->hasPostfix());
    BOOST_ASSERT(items[2]->hasPostfix());
}

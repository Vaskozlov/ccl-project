#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"([a-z]*p[_]p"test")" }, 0, shared);
    const auto &items = dot_item.getItems();

    assertTrue(items[0]->hasPrefix());
    assertTrue(items[1]->hasPrefix());
    assertTrue(not items[2]->hasPrefix());

    return {};
}
();

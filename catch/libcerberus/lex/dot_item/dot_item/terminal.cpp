#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared<char>{};
    auto dot_item = DotItem<char>(TextIterator<char>{ "[]'+'" }, 0, shared);

    assertEqual(shared.terminals.size(), 1_ZU);
    assertEqual(shared.terminals.matches("+").success, true);

    return {};
}
();

#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto dot_item = DotItem<char>(TextIterator<char>{ "(\"Test\")", nullptr, {}, "none" });
    return {};
}
();

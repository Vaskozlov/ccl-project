#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::text;
using namespace cerb::lex::dot_item;

STRING_TEST
{
    auto dot_item = DotItem<char>(TextIterator<char>{ "[a-z]*", nullptr, {}, "none" });
    return {};
}
();

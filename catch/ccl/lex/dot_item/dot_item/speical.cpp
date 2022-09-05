#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/container.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

BOOST_AUTO_TEST_CASE(SpecialItem)
{
    auto special_items = SpecialItems{};
    auto text_iterator = TextIterator{ "+" };

    special_items.special_items.emplace_back(
        std::make_unique<Container>(TextIterator{ R"(!"+")" }, special_items, 2));

    auto item = special_items.scan(text_iterator);

    fmt::print("{}\n", item->second.getRepr());
}

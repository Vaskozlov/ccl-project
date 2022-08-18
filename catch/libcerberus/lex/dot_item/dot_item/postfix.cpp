#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"([a-z]+[_]p"test"p)" }, 0, shared);
    const auto &items = dot_item.getItems();

    assertTrue(not items[0]->hasPrefix());
    assertTrue(not items[0]->hasPostfix());

    assertTrue(items[1]->hasPostfix());
    assertTrue(items[2]->hasPostfix());

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared{};

    try {
        auto dot_item = DotItem(TextIterator{ u8R"([a-z]+[_]p"test")" }, 0, shared);
        assertTrue(false);
    } catch (const cerb::lex::dot_item::DotItemException &exception) {
        assertEqual(exception.getColumn(), 16_ZU);// NOLINT

        assertEqual(
            exception.getMessage(),
            u8"unable to apply with reason: item without postfix modifier exists after items with "
            u8"them");

        assertEqual(
            exception.getSuggestion(),
            u8"add postfix modifier to the last item\n"
            u8"[a-z]+[_]p\"test\"p");
    }

    return {};
}
();

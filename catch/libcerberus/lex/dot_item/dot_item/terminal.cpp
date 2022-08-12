#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"('+' '-')" }, 0, shared);

    assertEqual(shared.terminals.size(), 2_ZU);

    assertEqual(shared.terminals.matches(u8"+").success, true);
    assertEqual(shared.terminals.matches(u8"-").success, true);

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared{};

    try {
        UNUSED_DECL DotItem(TextIterator{ u8R"([] '+' )" }, 0, shared);
        assertTrue(false);
    } catch (const DotItemException &exception) {
        assertEqual(exception.getColumn(), 4_ZU);// NOLINT
        assertEqual(
            exception.getMessage(),
            u8"unable to create terminal: terminals cannot coexist with other items");
    }

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared{};

    try {
        UNUSED_DECL DotItem(TextIterator{ u8R"('+' [])" }, 0, shared);
        assertTrue(false);
    } catch (const DotItemException &exception) {
        assertEqual(exception.getColumn(), 5_ZU);// NOLINT
        assertEqual(
            exception.getMessage(),
            u8"unable to create union: terminals cannot coexist with other items");
    }

    return {};
}
();

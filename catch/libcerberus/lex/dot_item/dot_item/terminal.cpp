#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared<char>{};
    auto dot_item = DotItem<char>(TextIterator<char>{ R"('+' '-')" }, 0, shared);

    assertEqual(shared.terminals.size(), 2_ZU);

    assertEqual(shared.terminals.matches("+").success, true);
    assertEqual(shared.terminals.matches("-").success, true);

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared<char>{};

    try {
        UNUSED_DECL DotItem<char>(TextIterator<char>{ R"([] '+' )" }, 0, shared);
        assertTrue(false);
    } catch (const DotItemException<char> &exception) {
        assertEqual(exception.getColumn(), 4_ZU);// NOLINT
        assertEqual(
            exception.getMessage(),
            "unable to create terminal: terminals cannot coexist with other items");
    }

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared<char>{};

    try {
        UNUSED_DECL DotItem<char>(TextIterator<char>{ R"('+' [])" }, 0, shared);
        assertTrue(false);
    } catch (const DotItemException<char> &exception) {
        assertEqual(exception.getColumn(), 5_ZU);// NOLINT
        assertEqual(
            exception.getMessage(),
            "unable to create union: terminals cannot coexist with other items");
    }

    return {};
}
();

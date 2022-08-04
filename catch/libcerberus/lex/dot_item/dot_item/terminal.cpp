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

    ERROR_EXPECTED(
        DotItem<char>(TextIterator<char>{ R"([] '+' )" }, 0, shared), TextIteratorException<char>,
        "Error occurred at: , line: 1, column: 4. Error message: unable to create terminal: "
        "terminals cannot coexist with other items\n"
        "[] '+' \n"
        "   ^\n"
        "Suggestion: do not declare terminal or delete other items");

    return {};
}
();

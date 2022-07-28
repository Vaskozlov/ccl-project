#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared<char>{};
    auto dot_item = DotItem<char>(TextIterator<char>{ "'+'" }, 0, shared);

    assertEqual(shared.terminals.size(), 1_ZU);
    assertEqual(shared.terminals.matches("+").success, true);

    return {};
}
();


// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared<char>{};

    ERROR_EXPECTED(DotItem<char>(TextIterator<char>{ "[]'+'" }, 0, shared);
                   , TextIteratorException<char>,
                   "Error occurred at: , line: 1, column: 3. Error message: dot item with terminal "
                   "must be empty\n"
                   "[]'+'\n"
                   "  ^\n"
                   "Suggestion: delete other items");

    return {};
}
();

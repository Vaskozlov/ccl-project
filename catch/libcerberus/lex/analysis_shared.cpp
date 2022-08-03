#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/analysis_shared.hpp>

using namespace cerb::lex;
using namespace cerb::text;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ "/* test */" };
    auto analysis_shared = AnalysisShared<char>{ { "//", "/*", "*/" } };

    assertTrue(analysis_shared.isComment(text_iterator.getRemainingFuture(1)));
    assertTrue(analysis_shared.isNextCharNotForScanning(text_iterator));

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ "+" };
    auto analysis_shared = AnalysisShared<char>{ {}, {}, { { "+", 1 } } };

    assertTrue(analysis_shared.isTerminal(text_iterator.getRemainingFuture(1)));
    assertTrue(analysis_shared.isNextCharNotForScanning(text_iterator));

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ R"("")" };
    auto analysis_shared = AnalysisShared<char>{ {}, { { "\"", 0, false, false } } };

    assertTrue(analysis_shared.isNextCharNotForScanning(text_iterator));

    return {};
}
();

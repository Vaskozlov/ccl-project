#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/analysis_shared.hpp>

using namespace cerb::lex;
using namespace cerb::text;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"/* test */" };
    auto analysis_shared = AnalysisShared{ { u8"//", u8"/*", u8"*/" } };

    assertTrue(analysis_shared.isComment(text_iterator.getFutureRemaining(1)));
    assertTrue(analysis_shared.isNextCharNotForScanning(text_iterator));

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"+" };
    auto analysis_shared = AnalysisShared{ {}, {}, { { u8"+", 1 } } };

    assertTrue(analysis_shared.isTerminal(text_iterator.getFutureRemaining(1)));
    assertTrue(analysis_shared.isNextCharNotForScanning(text_iterator));

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8R"("")" };
    auto analysis_shared = AnalysisShared{ {}, { { u8"\"", 0, false, false } } };

    assertTrue(analysis_shared.isNextCharNotForScanning(text_iterator));

    return {};
}
();

#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/sequence.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

auto shared = AnalysisShared<char>{};

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto exception_accumulator = TextIterator<char>::ExceptionAccumulator{};
    auto text_iterator = TextIterator<char>{ R"("")", &exception_accumulator };
    text_iterator.nextRawChar();

    auto string_item = Sequence<char>(false, "\"", text_iterator, shared);
    const auto &string = string_item.get();

    assertTrue(string.empty());
    assertEqual(exception_accumulator.getWarnings().size(), 1_ZU);

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ R"("Hello, \"World\"!")" };
    text_iterator.nextRawChar();

    auto string_item = Sequence<char>(false, "\"", text_iterator, shared);
    const auto &string = string_item.get();

    assertEqual(string, R"(Hello, "World"!)");

    return {};
}
();

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "\"\"\"Hello,\n    \"World\"!\"\"\"" };
    text_iterator.nextRawChar();

    auto string_item = Sequence<char>(true, R"(""")", text_iterator, shared);
    const auto &string = string_item.get();

    assertEqual(string, "Hello,\n    \"World\"!");
    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ R"("Hello, World!)" };
    text_iterator.nextRawChar();

    ERROR_EXPECTED(
        Sequence<char>(false, "\"", text_iterator, shared), TextIteratorException<char>,
        "Error occurred at: , line: 1, column: 1. Error message: unterminated string item\n"
        "\"Hello, World!\n"
        "^");

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ "\"Hello, World!\n\"" };
    text_iterator.nextRawChar();

    ERROR_EXPECTED(
        Sequence<char>(false, "\"", text_iterator, shared), TextIteratorException<char>,
        "Error occurred at: , line: 1, column: 1. Error message: New line reached, but string "
        "literal has not been terminated\n"
        "\"Hello, World!\n"
        "^\n"
        "Suggestion: use multiline option or close string literal with \"");

    return {};
}
();

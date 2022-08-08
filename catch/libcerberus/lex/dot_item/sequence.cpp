#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/sequence.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
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

    try {
        UNUSED_DECL Sequence<char>(false, "\"", text_iterator, shared);
        assertTrue(false);
    } catch (const SequenceException<char> &exception) {
        assertEqual(exception.getColumn(), 1_ZU);// NOLINT
        assertEqual(exception.getMessage(), "unterminated sequence");
    }

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ "\"Hello, World!\n\"" };
    text_iterator.nextRawChar();

    try {
        UNUSED_DECL Sequence<char>(false, "\"", text_iterator, shared);
        assertTrue(false);
    } catch (const SequenceException<char> &exception) {
        assertEqual(exception.getColumn(), 1_ZU);// NOLINT
        assertEqual(
            exception.getMessage(), "new line is reached, but sequence has not been terminated");
        assertEqual(exception.getSuggestion(), "use multiline sequence or close it with `\"`");
    }

    return {};
}
();

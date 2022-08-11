#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/sequence.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
auto shared = AnalysisShared{};

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto exception_accumulator = TextIterator::ExceptionAccumulator{};
    auto text_iterator = TextIterator{ u8R"("")", &exception_accumulator };
    text_iterator.nextRawChar();

    auto string_item = Sequence(false, u8"\"", text_iterator, shared);
    const auto &string = string_item.get();

    assertTrue(string.empty());
    assertEqual(exception_accumulator.getWarnings().size(), 1_ZU);

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8R"("Hello, \"World\"!")" };
    text_iterator.nextRawChar();

    auto string_item = Sequence(false, u8"\"", text_iterator, shared);
    const auto &string = string_item.get();

    assertEqual(string, u8R"(Hello, "World"!)");

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"\"\"\"Hello,\n    \"World\"!\"\"\"" };
    text_iterator.nextRawChar();

    auto string_item = Sequence(true, u8R"(""")", text_iterator, shared);
    const auto &string = string_item.get();

    assertEqual(string, u8"Hello,\n    \"World\"!");
    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8R"("Hello, World!)" };
    text_iterator.nextRawChar();

    try {
        UNUSED_DECL Sequence(false, u8"\"", text_iterator, shared);
        assertTrue(false);
    } catch (const SequenceException &exception) {
        assertEqual(exception.getColumn(), 1_ZU);// NOLINT
        assertEqual(exception.getMessage(), u8"unterminated sequence");
    }

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"\"Hello, World!\n\"" };
    text_iterator.nextRawChar();

    try {
        UNUSED_DECL Sequence(false, u8"\"", text_iterator, shared);
        assertTrue(false);
    } catch (const SequenceException &exception) {
        assertEqual(exception.getColumn(), 1_ZU);// NOLINT
        assertEqual(
            exception.getMessage(), u8"new line is reached, but sequence has not been terminated");
        assertEqual(exception.getSuggestion(), u8"use multiline sequence or close it with `\"`");
    }

    return {};
}
();

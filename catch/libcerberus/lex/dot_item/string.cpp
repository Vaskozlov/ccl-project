#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/dot_item/string.hpp>

using namespace cerb::text;
using namespace cerb::debug;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ R"("Hello, \"World\"!")" };
    text_iterator.nextRawChar();

    auto string_item = String<char>(false, "\"", text_iterator);
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

    auto string_item = String<char>(true, R"(""")", text_iterator);
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
        String<char>(false, "\"", text_iterator), TextIteratorException<char>,
        "Error occurred at: , line: 1, column: 1. Error message: unterminated string literal\n"
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
        String<char>(false, "\"", text_iterator), TextIteratorException<char>,
        "Error occurred at: , line: 1, column: 1. Error message: New line reached, but string "
        "literal has not been terminated\n"
        "\"Hello, World!\n"
        "^\n"
        "Suggestion: use multiline option or close string literal with \"");

    return {};
}
();

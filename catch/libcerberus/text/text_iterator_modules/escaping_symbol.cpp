#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator.hpp>
#include <cerberus/text/text_iterator_modules/escaping_symbol.hpp>

using namespace cerb::text;
using namespace cerb::debug;
using namespace cerb::string_view_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ R"(Test \n\t\wstring!)" };
    auto expected_text = "Test \n\tWstring!"_sv;

    for (auto chr : expected_text) {
        assertEqual(text_iterator.nextRawCharWithEscapingSymbols({ { 'w', 'W' } }), chr);
    }

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ R"(\w)" };

    ERROR_EXPECTED(
        UNUSED_DECL text_iterator.nextRawCharWithEscapingSymbols(), TextIteratorException<char>,
        "Error occurred at: , line: 1, column: 2, message: unable to match any escaping symbol\n"
        "\\w\n"
        " ^");

    return {};
}
();

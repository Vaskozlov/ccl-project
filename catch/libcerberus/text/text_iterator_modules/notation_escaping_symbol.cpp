#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator.hpp>
#include <cerberus/text/text_iterator_modules/escaping_symbol.hpp>

using namespace cerb::text;
using namespace cerb::debug;
using namespace cerb::string_view_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char>{ R"(\077\xFF\u0041)" };
    auto expected_text = "\077\xFF\u0041"_sv;

    for (auto chr : expected_text) {
        assertEqual(text_iterator.nextRawCharWithEscapingSymbols(), chr);
    }

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ R"(\u01FF)" };

    ERROR_EXPECTED(
        UNUSED_DECL text_iterator.nextRawCharWithEscapingSymbols(), TextIteratorException<char>,
        "Error occurred at: , line: 1, column: 6, message: character literal overflow\n"
        "\\u01FF\n"
        "     ^");

    return {};
}
();

#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator.hpp>
#include <cerberus/text/text_iterator_modules/escaping_symbol.hpp>

using namespace cerb::text;
using namespace cerb::string_view_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator<char, char16_t>{ R"(Test \n\t\wstring!)" };
    auto expected_text = u"Test \n\tWstring!"_sv;

    for (auto expected_chr : expected_text) {
        auto [is_escaping, chr] = text_iterator.nextRawCharWithEscapingSymbols({ { 'w', u'W' } });
        assertEqual(chr, expected_chr);
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
        "Error occurred at: , line: 1, column: 2. Error message: unable to match any escaping "
        "symbol\n"
        "\\w\n"
        " ^");

    return {};
}
();

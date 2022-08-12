#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator.hpp>
#include <cerberus/text/text_iterator_modules/escaping_symbol.hpp>

using namespace cerb::text;
using namespace cerb::string_view_literals;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ R"(\077\xFF\u041)" };
    auto expected_text = "\077\xFF\u0041"_sv;

    try {
        for (auto expected_chr : expected_text) {
            auto [is_escaping, chr] = text_iterator.nextRawCharWithEscapingSymbols();
            assertEqual(chr, expected_chr);
        }
        assertTrue(false);
    } catch (const TextIteratorException<char> &error) {
        assertEqual(error.getColumn(), 13_ZU);// NOLINT
        assertEqual(error.getMessage(), "expected 4 characters, but only 3 of them were provided");
        assertEqual(error.getSuggestion(), R"(\077\xFF\u0041)");
    }

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator<char16_t, char>{ uR"(\u01FF)" };

    try {
        UNUSED_DECL text_iterator.nextRawCharWithEscapingSymbols();
        assertTrue(false);
    } catch (const module::NotationEscapingSymbolizerException<char16_t> &exception) {
        assertEqual(exception.getColumn(), 6_ZU);// NOLINT
        assertEqual(exception.getMessage(), u"character literal overflow");
    }

    return {};
}
();

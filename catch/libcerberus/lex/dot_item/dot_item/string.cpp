#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"("\'"c)" }, 0, shared);

    assertEqual(shared.strings_and_chars.size(), 1_ZU);
    assertEqual(shared.strings_and_chars[0].str_begin, u8R"(')");
    assertEqual(shared.strings_and_chars[0].str_end, u8R"(')");
    assertEqual(shared.strings_and_chars[0].is_multiline, false);
    assertEqual(shared.strings_and_chars[0].is_character, true);

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"( "R\"(:)\""c )" }, 0, shared);

    assertEqual(shared.strings_and_chars.size(), 1_ZU);
    assertEqual(shared.strings_and_chars[0].str_begin, u8R"(R"()");
    assertEqual(shared.strings_and_chars[0].str_end, u8")\"");
    assertEqual(shared.strings_and_chars[0].is_multiline, false);
    assertEqual(shared.strings_and_chars[0].is_character, true);

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared{};
    auto dot_item = DotItem(TextIterator{ u8R"("\""s "'"s)" }, 0, shared);

    assertEqual(shared.strings_and_chars.size(), 2_ZU);

    assertEqual(shared.strings_and_chars[0].str_begin, u8R"(")");
    assertEqual(shared.strings_and_chars[1].str_end, u8R"(')");

    for (auto &string : shared.strings_and_chars) {
        assertEqual(string.is_multiline, false);
        assertEqual(string.is_character, false);
    }

    return {};
}
();

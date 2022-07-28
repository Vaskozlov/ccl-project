#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared<char>{};
    auto dot_item = DotItem<char>(TextIterator<char>{ R"("\'\'"c)" }, 0, shared);

    assertEqual(shared.strings_and_chars.size(), 1_ZU);
    assertEqual(shared.strings_and_chars[0].str, R"('')");
    assertEqual(shared.strings_and_chars[0].is_multiline, false);
    assertEqual(shared.strings_and_chars[0].is_character, true);

    return {};
}
();

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto shared = AnalysisShared<char>{};
    auto dot_item = DotItem<char>(TextIterator<char>{ R"("\"\""s)" }, 0, shared);

    assertEqual(shared.strings_and_chars.size(), 1_ZU);
    assertEqual(shared.strings_and_chars[0].str, R"("")");
    assertEqual(shared.strings_and_chars[0].is_multiline, false);
    assertEqual(shared.strings_and_chars[0].is_character, false);

    return {};
}
();

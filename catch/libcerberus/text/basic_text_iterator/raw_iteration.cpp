#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb;
using namespace cerb::text;
using namespace cerb::string_view_literals;

template<CharacterLiteral CharT>
constexpr auto testRawIteration() -> bool
{
    auto raw_input = strCast<CharT>("Hello, \t\nWorld!");
    auto input = BasicStringView{ raw_input };

    auto raw_expected = strCast<CharT>("Hello, \t\nWorld!");
    auto expected = BasicStringView{ raw_expected };

    auto text_iterator = BasicTextIterator{ input };

    for (const auto &chr : expected) {
        assertEqual(text_iterator.nextRawChar(), chr);
        assertEqual(text_iterator.getCurrentChar(), chr);
    }

    return true;
}

static_assert(testRawIteration<char>());
static_assert(testRawIteration<char16_t>());

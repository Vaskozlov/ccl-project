#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb;
using namespace cerb::text;
using namespace cerb::debug;
using namespace cerb::string_view_literals;

template<cerb::CharacterLiteral CharT>
constexpr auto testCleanIteration() -> bool
{
    auto raw_input = strCast<CharT>("Hello, \t\nWorld!");
    auto input = BasicStringView{ raw_input };

    auto raw_expected = strCast<CharT>("Hello,World!");
    auto expected = BasicStringView{ raw_expected };

    auto text_iterator = BasicTextIterator{ input };

    for (const auto &chr : expected) {
        assertEqual(text_iterator.nextCleanChar(), chr);
        assertEqual(text_iterator.getCurrentChar(), chr);
    }

    return true;
}

static_assert(testCleanIteration<char>());
static_assert(testCleanIteration<char16_t>());

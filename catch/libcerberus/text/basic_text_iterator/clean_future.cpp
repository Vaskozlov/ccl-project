#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb;
using namespace cerb::text;
using namespace cerb::debug;
using namespace cerb::integral_literals;
using namespace cerb::string_view_literals;

template<CharacterLiteral CharT>
constexpr auto testCleanFuture() -> bool
{
    auto raw_input = strCast<CharT>("Hello, \t\nWorld!");
    auto input = BasicStringView{ raw_input };

    auto raw_expected = strCast<CharT>("Hello,World!");
    auto expected = BasicStringView{ raw_expected };

    auto index = 1_ZU;
    auto text_iterator = BasicTextIterator{ input };

    for (const auto &chr : expected) {
        assertEqual(text_iterator.futureCleanChar(index), chr);
        ++index;
    }

    return true;
}

static_assert(testCleanFuture<char>());
static_assert(testCleanFuture<char16_t>());

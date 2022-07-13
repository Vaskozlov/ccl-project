#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::debug;
using namespace cerb::integral_literals;
using namespace cerb::string_view_literals;

template<cerb::CharacterLiteral CharT>
constexpr auto testRawFuture() -> bool
{
    auto casted_in = cerb::strCast<CharT>("Hello, \t\nWorld!");
    auto input = cerb::BasicStringView{ casted_in.data(), casted_in.size() };

    auto casted_ex = cerb::strCast<CharT>("Hello, \t\nWorld!");
    auto expected = cerb::BasicStringView{ casted_ex.data(), casted_ex.size() };

    auto index = 1_ZU;
    auto text_iterator = BasicTextIterator{ input };

    for (const auto &chr : expected) {
        assertEqual(text_iterator.futureRawChar(index), chr);
        ++index;
    }

    return true;
}

static_assert(testRawFuture<char>());
static_assert(testRawFuture<char16_t>());

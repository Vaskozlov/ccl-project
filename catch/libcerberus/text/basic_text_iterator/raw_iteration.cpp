#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::debug;
using namespace cerb::string_view_literals;

template<cerb::CharacterLiteral CharT>
constexpr auto testRawIteration() -> bool
{
    auto casted_in = cerb::strCast<CharT>("Hello, \t\nWorld!");
    auto input = cerb::BasicStringView{ casted_in.data(), casted_in.size() };

    auto casted_ex = cerb::strCast<CharT>("Hello, \t\nWorld!");
    auto expected = cerb::BasicStringView{ casted_ex.data(), casted_ex.size() };

    auto text_iterator = BasicTextIterator{ input };

    for (const auto &chr : expected) {
        assertEqual(chr, text_iterator.nextRawChar());
        assertEqual(chr, text_iterator.getCurrentChar());
    }

    return true;
}

static_assert(testRawIteration<char>());
static_assert(testRawIteration<char16_t>());

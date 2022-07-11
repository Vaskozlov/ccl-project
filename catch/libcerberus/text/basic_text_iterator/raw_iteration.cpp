#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::debug;
using namespace std::string_view_literals;

constexpr auto testRawIteration(auto input, auto expected) -> bool
{
    auto text_iterator = BasicTextIterator{ input };

    for (const auto &chr : expected) {
        assertEqual(chr, text_iterator.nextRawChar());
        assertEqual(chr, text_iterator.getCurrentChar());
    }

    return true;
}

static_assert(testRawIteration("Hello, \t\nWorld!"sv, "Hello, \t\nWorld!"sv));
static_assert(testRawIteration(u"Hello, \t\nWorld!"sv, u"Hello, \t\nWorld!"sv));
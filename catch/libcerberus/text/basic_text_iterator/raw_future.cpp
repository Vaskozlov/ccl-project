#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::integral_literals;
using namespace std::string_view_literals;

constexpr auto testRawFuture(auto input, auto expected) -> bool
{
    auto index = 1_ZU;
    auto text_iterator = BasicTextIterator{ input };

    for (const auto &chr : expected) {
        ASSERT_EQ(chr, text_iterator.futureRawChar(index));
        ++index;
    }

    return true;
}

static_assert(testRawFuture("Hello, \t\nWorld!"sv, "Hello, \t\nWorld!"sv));
static_assert(testRawFuture(u"Hello, \t\nWorld!"sv, u"Hello, \t\nWorld!"sv));
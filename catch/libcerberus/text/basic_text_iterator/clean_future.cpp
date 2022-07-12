#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::debug;
using namespace cerb::integral_literals;
using namespace cerb::string_view_literals;

constexpr auto testCleanFuture(auto input, auto expected) -> bool
{
    auto index = 1_ZU;
    auto text_iterator = BasicTextIterator{ input };

    for (const auto &chr : expected) {
        assertEqual(chr, text_iterator.futureCleanChar(index));
        ++index;
    }

    return true;
}

static_assert(testCleanFuture("Hello, \t\nWorld!"_sv, "Hello,World!"_sv));
static_assert(testCleanFuture(u"Hello, \t\nWorld!"_sv, u"Hello,World!"_sv));

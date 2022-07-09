#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::text;
using namespace std::string_view_literals;

constexpr auto testCleanIteration(auto input, auto expected) -> bool
{
    auto text_iterator = BasicTextIterator{ input };

    for (const auto &chr : expected) {
        ASSERT_EQ(chr, text_iterator.nextCleanChar());
        ASSERT_EQ(chr, text_iterator.getCurrentChar());
    }

    return true;
}

static_assert(testCleanIteration("Hello, \t\nWorld!"sv, "Hello,World!"sv));
static_assert(testCleanIteration(u"Hello, \t\nWorld!"sv, u"Hello,World!"sv));
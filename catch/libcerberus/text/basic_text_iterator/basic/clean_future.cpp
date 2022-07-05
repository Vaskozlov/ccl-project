#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

// NOLINTBEGIN

using namespace cerb::integral_literals;
using namespace std::string_view_literals;

using BasicTextIterator = cerb::text::BasicTextIterator<char>;

STATIC_VARIABLE auto InputString = "Hello, \t\nWorld!"sv;
STATIC_VARIABLE auto ExpectedString = "Hello,World!"sv;

DEBUG_SCOPE
{
    auto index = 1_ZU;
    auto text_iterator = BasicTextIterator{ InputString };

    for (auto &chr : ExpectedString) {
        ASSERT_EQ(chr, text_iterator.futureCleanChar(index));
        ++index;
    }

    return {};
}

// NOLINTEND

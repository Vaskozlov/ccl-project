#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::text;
using namespace std::string_view_literals;

// NOLINTBEGIN

STATIC_VARIABLE auto InputString = "Hello, World!"sv;
STATIC_VARIABLE auto ExpectedString = "Hello, World!"sv;

DEBUG_SCOPE
{
    size_t index = 1;
    auto text_iterator = BasicTextIterator<char>{ InputString };

    for (auto &chr : ExpectedString) {
        ASSERT_EQ(chr, text_iterator.futureRawChar(index));
        ++index;
    }

    return {};
}

// NOLINTEND

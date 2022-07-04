#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::text;
using namespace std::string_view_literals;

// NOLINTBEGIN

DEBUG_SCOPE
{
    STATIC_VARIABLE auto input_string = "Hello, World!"sv;
    STATIC_VARIABLE auto expected_string = "Hello, World!"sv;

    size_t index = 1;
    auto text_iterator = BasicTextIterator<char>{ input_string };

    for (auto &chr : expected_string) {
        ASSERT_EQ(chr, text_iterator.futureRawChar(index));
        ++index;
    }

    return {};
}

// NOLINTEND

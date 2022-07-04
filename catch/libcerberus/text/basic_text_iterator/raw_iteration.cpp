#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::text;
using namespace std::string_view_literals;

// NOLINTBEGIN

DEBUG_SCOPE
{
    constexpr auto input_string = "Hello, World!"sv;

    auto text_iterator = BasicTextIterator<char>{ input_string };

    for (auto &chr : input_string) {
        ASSERT_EQ(chr, text_iterator.nextRawChar());
        ASSERT_EQ(chr, text_iterator.getCurrent());
    }

    return {};
}

// NOLINTEND

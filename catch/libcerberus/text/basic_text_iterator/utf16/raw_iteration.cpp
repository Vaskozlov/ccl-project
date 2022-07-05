#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

// NOLINTBEGIN

using namespace std::string_view_literals;

using BasicTextIterator = cerb::text::BasicTextIterator<char16_t>;

STATIC_VARIABLE auto InputString = u"Hello, World!"sv;
STATIC_VARIABLE auto ExpectedString = u"Hello, World!"sv;

DEBUG_SCOPE
{
    auto text_iterator = BasicTextIterator{ InputString };

    for (auto &chr : ExpectedString) {
        ASSERT_EQ(chr, text_iterator.nextRawChar());
        ASSERT_EQ(chr, text_iterator.getCurrentChar());
    }

    return {};
}

// NOLINTEND

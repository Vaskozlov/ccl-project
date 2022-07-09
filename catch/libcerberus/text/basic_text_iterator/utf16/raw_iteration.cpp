#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace std::string_view_literals;
using BasicTextIterator = cerb::text::BasicTextIterator<char16_t>;

CONSTEXPR_TEST
{
    auto text_iterator = BasicTextIterator{ u"Hello, World!"sv };

    for (const auto &chr : u"Hello, World!"sv) {
        ASSERT_EQ(chr, text_iterator.nextRawChar());
        ASSERT_EQ(chr, text_iterator.getCurrentChar());
    }

    return {};
}
();

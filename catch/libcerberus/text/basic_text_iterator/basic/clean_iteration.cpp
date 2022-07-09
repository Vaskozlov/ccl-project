#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace std::string_view_literals;
using BasicTextIterator = cerb::text::BasicTextIterator<char>;

CONSTEXPR_TEST
{
    auto text_iterator = BasicTextIterator{ "Hello, \t\nWorld!"sv };

    for (const auto &chr : "Hello,World!"sv) {
        ASSERT_EQ(chr, text_iterator.nextCleanChar());
        ASSERT_EQ(chr, text_iterator.getCurrentChar());
    }

    return {};
}
();

#include <cerberus/debug/debug.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::integral_literals;
using namespace std::string_view_literals;
using BasicTextIterator = cerb::text::BasicTextIterator<char>;

CONSTEXPR_TEST
{
    auto index = 1_ZU;
    auto text_iterator = BasicTextIterator{ "Hello, \t\nWorld!"sv };

    for (const auto &chr : "Hello,World!"sv) {
        ASSERT_EQ(chr, text_iterator.futureCleanChar(index));
        ++index;
    }

    return {};
}
();

#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::string_view_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    auto input = u8"Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;
    auto expected = U"Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;

    auto index = 1_ZU;
    auto text_iterator = BasicTextIterator{ input };

    for (const auto &chr : expected) {
        assertEqual(text_iterator.futureRawChar(index), chr);
        ++index;
    }

    return {};
}
();

#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::string_view_literals;

BOOST_AUTO_TEST_CASE(BasicTextIteratorRawFuture)
{
    auto input = u8"Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;
    auto expected = U"Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;

    auto index = static_cast<size_t>(1);
    auto text_iterator = BasicTextIterator{ input };

    for (DEBUG_DECL &&chr : expected) {
        BOOST_ASSERT(text_iterator.futureRawChar(index) == chr);
        ++index;
    }
}

#include <boost/test/unit_test.hpp>
#include <cerberus/text/basic_text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::string_view_literals;

BOOST_AUTO_TEST_CASE(BasicTextIteratorRawIteration)
{
    auto input = u8"Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;
    auto expected = U"Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;
    auto text_iterator = BasicTextIterator{ input };

    for (const auto &chr : expected) {
        auto sym = text_iterator.nextRawChar();

        BOOST_ASSERT(sym == chr);
        BOOST_ASSERT(text_iterator.getCurrentChar() == chr);
    }
}

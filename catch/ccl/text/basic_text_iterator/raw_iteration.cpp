#include <ccl/debug/debug_file.hpp>
#include <ccl/text/basic_text_iterator.hpp>

using namespace ccl;
using namespace text;
using namespace string_view_literals;

BOOST_AUTO_TEST_CASE(BasicTextIteratorRawIteration)
{
    auto input = "Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;
    auto expected = U"Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;
    auto text_iterator = BasicTextIterator{ input };

    for (DEBUG_VAR &&chr : expected) {
        DEBUG_VAR sym = text_iterator.next();

        BOOST_ASSERT(sym == chr);
        BOOST_ASSERT(text_iterator.getCurrentChar() == chr);
    }
}

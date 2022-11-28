#include <ccl/debug/debug_file.hpp>
#include <ccl/text/basic_text_iterator.hpp>

using namespace ccl;
using namespace text;
using namespace string_view_literals;

BOOST_AUTO_TEST_CASE(BasicTextIteratorRawFuture)
{
    auto input = "Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;
    auto expected = U"Hello, \t\nWorld! \u00FF \uFFFF \U000FFFFF"_sv;

    DEBUG_VAR index = as<size_t>(1);
    DEBUG_VAR text_iterator = BasicTextIterator{input};

    for (DEBUG_VAR &&chr : expected) {
        BOOST_ASSERT(text_iterator.futureChar(index) == chr);
        ++index;
    }
}

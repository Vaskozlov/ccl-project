#include <ccl/debug/debug_file.hpp>
#include <ccl/text/text_iterator.hpp>

using namespace ccl;
using namespace text;
using namespace string_view_literals;

BOOST_AUTO_TEST_CASE(TextIteratorLineTracking)
{
    auto text_iterator = TextIterator{ u8"Hi\nWorld!"_sv };

    for (size_t i = 0; i != 3; ++i) {
        text_iterator.next();
        BOOST_ASSERT(text_iterator.getWorkingLine() == u8"Hi");
    }

    text_iterator.next();
    BOOST_ASSERT(text_iterator.getWorkingLine() == u8"World!");
}

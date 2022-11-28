#include <ccl/debug/debug_file.hpp>
#include <ccl/text/text_iterator.hpp>

using namespace ccl;
using namespace text;
using namespace string_view_literals;

BOOST_AUTO_TEST_CASE(TextIteratorLineTracking)
{
    auto text_iterator = TextIterator{"Hi\nWorld!"_sv};

    for (auto i = as<size_t>(0); i != 3; ++i) {
        text_iterator.next();
        BOOST_ASSERT(text_iterator.getWorkingLine() == "Hi");
    }

    text_iterator.next();
    BOOST_ASSERT(text_iterator.getWorkingLine() == "World!");
}

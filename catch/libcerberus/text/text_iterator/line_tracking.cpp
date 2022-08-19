#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::string_view_literals;

BOOST_AUTO_TEST_CASE(TextIteratorLineTracking)
{
    auto text_iterator = TextIterator{ u8"Hi\nWorld!"_sv };

    for (size_t i = 0; i != 3; ++i) {
        text_iterator.nextRawChar();
        BOOST_ASSERT(text_iterator.getWorkingLine() == u8"Hi");
    }

    text_iterator.nextRawChar();
    BOOST_ASSERT(text_iterator.getWorkingLine() == u8"World!");
}

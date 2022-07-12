#include <cerberus/debug/debug.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::debug;
using namespace cerb::integral_literals;
using namespace cerb::string_view_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator{ "Hi\nWorld!"_sv };

    for (size_t i = 0; i != 3; ++i) {
        text_iterator.nextRawChar();
        assertEqual(text_iterator.getWorkingLine(), "Hi");
    }

    text_iterator.nextRawChar();
    assertEqual(text_iterator.getWorkingLine(), "World!");

    return {};
}
();

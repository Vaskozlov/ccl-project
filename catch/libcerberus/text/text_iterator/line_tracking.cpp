#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::string_view_literals;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"Hi\nWorld!"_sv };

    for (size_t i = 0; i != 3; ++i) {
        text_iterator.nextRawChar();
        assertEqual(text_iterator.getWorkingLine(), u8"Hi");
    }

    text_iterator.nextRawChar();
    assertEqual(text_iterator.getWorkingLine(), u8"World!");

    return {};
}
();

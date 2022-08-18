#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::string_view_literals;

// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator{ u8"\t\t  H i\nWorld!"_sv };

    text_iterator.moveToCleanChar();
    text_iterator.nextRawChar();

    assertEqual(text_iterator.getTabsAndSpaces(), u8"\t\t  ");

    text_iterator.moveToCleanChar();
    text_iterator.nextRawChar();

    assertEqual(text_iterator.getTabsAndSpaces(), u8" ");

    return {};
}
();

#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;
using namespace cerb::string_view_literals;

// NOLINTNEXTLINE
STRING_TEST
{
    auto text_iterator = TextIterator{ "\t\t  H i\nWorld!"_sv };

    text_iterator.nextCleanChar();
    assertEqual(text_iterator.getTabsAndSpaces(), "\t\t  ");

    text_iterator.nextCleanChar();
    assertEqual(text_iterator.getTabsAndSpaces(), " ");

    return {};
}
();

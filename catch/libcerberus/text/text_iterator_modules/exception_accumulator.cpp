#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator.hpp>
#include <cerberus/text/text_iterator_modules/exception.hpp>

using namespace cerb::text;
using namespace cerb::string_view_literals;


// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator =
        TextIterator{ "Hello, World!\nIt's a test string!"_sv, {}, {}, "builtin"_sv };
    auto exception = TextIteratorException{ text_iterator, "some message"_sv };

    assertEqual(exception.getColumn(), 0_ZU);
    assertEqual(exception.getMessage(), "some message");

    assertEqual(
        exception.getFullMessage(),
        "Error occurred at: builtin, line: 1, column: 0, message: some message\n"
        "Hello, World!\n"
        "^");

    return {};
}
();

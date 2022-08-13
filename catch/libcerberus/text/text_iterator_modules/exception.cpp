#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/iterator_exception.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;


// NOLINTNEXTLINE
RUNTIME_TEST
{
    auto text_iterator = TextIterator(u8"Hello, World!\nIt's a test string!", {}, {}, u8"builtin");
    auto exception = TextIteratorException(
        text_iterator.getLocation(), text_iterator.getWorkingLine(), u8"some message");

    assertEqual(exception.getColumn(), 0_ZU);
    assertEqual(exception.getMessage(), u8"some message");

    assertEqual(
        exception.createFullMessage(),
        u8"Error occurred at: builtin, line: 1, column: 0, message: some message\n"
        "Hello, World!\n"
        "^");

    return {};
}
();

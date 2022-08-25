#include <ccl/debug/debug_file.hpp>
#include <ccl/text/iterator_exception.hpp>
#include <ccl/text/text_iterator.hpp>

using namespace ccl;
using namespace text;

BOOST_AUTO_TEST_CASE(TextIteratorExceptionConstruction)
{
    auto text_iterator = TextIterator(u8"Hello, World!\nIt's a test string!", {}, {}, u8"builtin");
    auto exception = TextIteratorException(
        text_iterator.getLocation(), text_iterator.getWorkingLine(), u8"some message");

    BOOST_ASSERT(exception.getColumn() == 0);
    BOOST_ASSERT(exception.getMessage() == u8"some message");

    BOOST_ASSERT(
        exception.createFullMessage() ==
        u8"Error occurred at: builtin, line: 1, column: 0, message: some message\n"
        "Hello, World!\n"
        "^");
}

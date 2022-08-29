#include <ccl/debug/debug_file.hpp>
#include <ccl/text/iterator_exception.hpp>
#include <ccl/text/text_iterator.hpp>

using namespace ccl;
using namespace text;

BOOST_AUTO_TEST_CASE(TextIteratorExceptionConstruction)
{
    auto text_iterator = TextIterator(
        "Hello, World!\nIt's a test string!", ExceptionHandler::instance(), {}, "builtin");
    auto exception = TextIteratorException(
        text_iterator.getLocation(), text_iterator.getWorkingLine(), "some message");

    BOOST_ASSERT(exception.getColumn() == 0);
    BOOST_ASSERT(exception.getMessage() == "some message");

    BOOST_ASSERT(
        exception.createFullMessage() ==
        "Error occurred at: builtin, line: 1, column: 0, message: some message\n"
        "Hello, World!\n"
        "^");
}

#include <ccl/debug/debug_file.hpp>
#include <ccl/text/iterator_exception.hpp>
#include <ccl/text/text_iterator.hpp>

using namespace ccl;
using namespace text;

TEST_CASE("TextIteratorExceptionConstruction", "[TextIterator]")
{
    auto text_iterator =
        TextIterator("Hello, World!\nIt's a test string!", ExceptionHandler::instance(), "builtin");

    auto exception = TextIteratorException(
        ExceptionCriticality::UNCRITICAL, AnalysisStage::LEXICAL_ANALYSIS,
        text_iterator.getLocation(), 1, text_iterator.getWorkingLine(), "some message");

    REQUIRE(exception.getColumn() == 0);
    REQUIRE(exception.getMessage() == "some message");

    REQUIRE(
        exception.createFullMessage() ==
        "Error occurred at: builtin, line: 1, column: 0, message: some message\n"
        "Hello, World!\n"
        "^");
}

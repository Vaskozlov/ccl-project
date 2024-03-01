#include <ccl/debug/debug_file.hpp>
import ccl.text;

TEST_CASE("TextIteratorExceptionConstruction", "[TextIterator]")
{
    using namespace ccl;

    auto text_iterator = text::TextIterator(
        "Hello, World!\nIt's a test string!", handler::ExceptionHandler::instance(), "builtin");

    auto exception = text::TextIteratorException(
        text::ExceptionCriticality::UNCRITICAL, text::AnalysisStage::LEXICAL_ANALYSIS,
        text_iterator.getLocation(), 1, text_iterator.getWorkingLine(), "some message");

    REQUIRE(exception.getColumn() == 0);
    REQUIRE(exception.getMessage() == "some message");

    REQUIRE(
        exception.createFullMessage() ==
        "Error occurred at: builtin, line: 1, column: 0, message: some message\n"
        "Hello, World!\n"
        "^");
}

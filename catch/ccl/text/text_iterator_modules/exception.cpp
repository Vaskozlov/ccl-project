#include <ccl/debug/debug.hpp>
#include <ccl/text/iterator_exception.hpp>
#include <ccl/text/text_iterator.hpp>

TEST_CASE("TextIteratorExceptionConstruction", "[TextIterator]")
{
    using namespace ccl;

    const auto text_iterator = text::TextIterator(
        "Hello, World!\nIt's a test string!", ExceptionHandler::instance(), "builtin");

    const auto exception = text::TextIteratorException{
        ExceptionCriticality::UNCRITICAL,
        AnalysisStage::LEXICAL_ANALYSIS,
        text_iterator.getLocation(),
        "builtin",
        1,
        static_cast<std::string>(text_iterator.getCurrentLine()),
        "some message"};

    REQUIRE(exception.getColumn() == 0);
    REQUIRE(exception.getMessage() == "some message");

    REQUIRE(
        exception.createFullMessage() ==
        "Error occurred at: builtin, line: 1, column: 0, message: some message\n"
        "Hello, World!\n"
        "^");
}

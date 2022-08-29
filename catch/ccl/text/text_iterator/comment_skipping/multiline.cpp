#include <ccl/debug/debug_file.hpp>
#include <ccl/text/text_iterator.hpp>

using namespace ccl;
using namespace text;

BOOST_AUTO_TEST_SUITE(TextIteratorMultilineComment)

BOOST_AUTO_TEST_CASE(TextIteratorUnterminatedMultilineComment)
{
    auto text_iterator = TextIterator{ "/*", ExceptionHandler::instance(), { "//", "/*", "*/" } };

    BOOST_CHECK_EXCEPTION(
        text_iterator.skipCommentsAndLayout(), text::TextIteratorException,
        []([[maybe_unused]] const text::TextIteratorException &exception) {
            BOOST_ASSERT(exception.getColumn() == 2);// NOLINT
            BOOST_ASSERT(exception.getMessage() == "unterminated multiline comment");
            return true;
        });
}

BOOST_AUTO_TEST_CASE(TextIteratorMultilineCommentSkipping)
{
    auto text_iterator =
        TextIterator{ "/**/Hi!", ExceptionHandler::instance(), { "//", "/*", "*/" } };
    text_iterator.skipCommentsAndLayout();

    BOOST_ASSERT(text_iterator.next() == U'H');
}

BOOST_AUTO_TEST_CASE(TextIteratorMultilineCommentSkippingWithMultilineSymbols)
{
    auto text_iterator =
        TextIterator{ "/*1521\n\n151t*/\nHi!", ExceptionHandler::instance(), { "//", "/*", "*/" } };
    text_iterator.skipCommentsAndLayout();

    BOOST_ASSERT(text_iterator.next() == U'H');
}

BOOST_AUTO_TEST_CASE(TextIteratorMultilineTwoCommentSkipping)
{
    auto text_iterator = TextIterator{ "/*1521\n\n151t*/\n/* */ Hi!",
                                       ExceptionHandler::instance(),
                                       { "//", "/*", "*/" } };
    text_iterator.skipCommentsAndLayout();

    BOOST_ASSERT(text_iterator.next() == U'H');
}

BOOST_AUTO_TEST_SUITE_END()

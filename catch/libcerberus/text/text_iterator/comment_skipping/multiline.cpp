#include <cerberus/debug/debug_file.hpp>
#include <cerberus/text/text_iterator.hpp>

using namespace cerb::text;

BOOST_AUTO_TEST_SUITE(TextIteratorMultilineComment)

BOOST_AUTO_TEST_CASE(TextIteratorUnterminatedMultilineComment)
{
    auto text_iterator = TextIterator{ u8"/*", nullptr, { u8"//", u8"/*", u8"*/" } };

    BOOST_CHECK_EXCEPTION(
        text_iterator.skipCommentsAndLayout(), CommentSkipperException,
        []([[maybe_unused]] const CommentSkipperException &exception) {
            BOOST_ASSERT(exception.getColumn() == 2);// NOLINT
            BOOST_ASSERT(exception.getMessage() == u8"unterminated multiline comment");
            return true;
        });
}

BOOST_AUTO_TEST_CASE(TextIteratorMultilineCommentSkipping)
{
    auto text_iterator = TextIterator{ u8"/**/Hi!", nullptr, { u8"//", u8"/*", u8"*/" } };
    text_iterator.skipCommentsAndLayout();

    BOOST_ASSERT(text_iterator.nextRawChar() == U'H');
}

BOOST_AUTO_TEST_CASE(TextIteratorMultilineCommentSkippingWithMultilineSymbols)
{
    auto text_iterator =
        TextIterator{ u8"/*1521\n\n151t*/\nHi!", nullptr, { u8"//", u8"/*", u8"*/" } };
    text_iterator.skipCommentsAndLayout();

    BOOST_ASSERT(text_iterator.nextRawChar() == U'H');
}

BOOST_AUTO_TEST_CASE(TextIteratorMultilineTwoCommentSkipping)
{
    auto text_iterator =
        TextIterator{ u8"/*1521\n\n151t*/\n/* */ Hi!", nullptr, { u8"//", u8"/*", u8"*/" } };
    text_iterator.skipCommentsAndLayout();

    BOOST_ASSERT(text_iterator.nextRawChar() == U'H');
}

BOOST_AUTO_TEST_SUITE_END()

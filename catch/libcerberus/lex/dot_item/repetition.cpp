#include <boost/test/unit_test.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::text;
using namespace cerb::lex::dot_item;

BOOST_AUTO_TEST_SUITE(DotItemRepetition)

BOOST_AUTO_TEST_CASE(RepetitionBasicCase)
{
    auto text_iterator = TextIterator{ u8"{10, 20}" };
    text_iterator.nextRawChar();

    auto repetition = Repetition{ text_iterator };
    BOOST_ASSERT(repetition.from == 10);
    BOOST_ASSERT(repetition.to == 20);
}

BOOST_AUTO_TEST_CASE(RepetitionEmptyFirstArgument)
{
    auto text_iterator = TextIterator{ u8"{, 1}" };
    text_iterator.nextRawChar();

    auto repetition = Repetition{ text_iterator };
    BOOST_ASSERT(repetition.from == 0);
    BOOST_ASSERT(repetition.to == 1);
}

BOOST_AUTO_TEST_CASE(RepetitionFirstArgumentGreaterThanSecond)
{
    auto text_iterator = TextIterator{ u8"{2, 1}" };
    text_iterator.nextRawChar();

    BOOST_CHECK_EXCEPTION(
        Repetition{ text_iterator }, RepetitionException, [](const RepetitionException &exception) {
            BOOST_ASSERT(exception.getColumn() == 1);
            BOOST_ASSERT(
                exception.getMessage() ==
                u8"the beginning of the repetition (2) is greater than the end (1)");
            return true;
        });
}

BOOST_AUTO_TEST_SUITE_END()

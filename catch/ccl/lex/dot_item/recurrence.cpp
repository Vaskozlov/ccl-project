#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/dot_item.hpp>

using namespace ccl;
using namespace text;
using namespace lex::dot_item;

BOOST_AUTO_TEST_SUITE(DotItemRecurrence)

BOOST_AUTO_TEST_CASE(RecurrenceBasicCase)
{
    auto text_iterator = TextIterator{ u8"{10, 20}" };
    text_iterator.next();

    DEBUG_VAR recurrence = Recurrence{ text_iterator };
    BOOST_ASSERT(recurrence.from == 10);
    BOOST_ASSERT(recurrence.to == 20);
}

BOOST_AUTO_TEST_CASE(RecurrenceEmptyFirstArgument)
{
    auto text_iterator = TextIterator{ u8"{, 1}" };
    text_iterator.next();

    DEBUG_VAR recurrence = Recurrence{ text_iterator };

    BOOST_ASSERT(recurrence.from == 0);
    BOOST_ASSERT(recurrence.to == 1);
}

BOOST_AUTO_TEST_CASE(RecurrenceFirstArgumentGreaterThanSecond)
{
    auto text_iterator = TextIterator{ u8"{2, 1}" };
    text_iterator.next();

    BOOST_CHECK_EXCEPTION(
        Recurrence{ text_iterator },
        RecurrenceException,
        []([[maybe_unused]] const RecurrenceException &exception) {
            BOOST_ASSERT(exception.getColumn() == 1);
            BOOST_ASSERT(
                exception.getMessage() ==
                u8"the beginning of the recurrence (2) is greater than the end (1)");
            return true;
        });
}

BOOST_AUTO_TEST_SUITE_END()

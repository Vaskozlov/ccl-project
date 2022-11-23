#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/container.hpp>

using namespace ccl;
using namespace text;
using namespace lex::dot_item;

BOOST_AUTO_TEST_SUITE(ContainerRepetition)

BOOST_AUTO_TEST_CASE(RepetitionBasicCase)
{
    auto text_iterator = TextIterator{"{10, 20}"};
    text_iterator.next();

    DEBUG_VAR repetition = Repetition{text_iterator};
    BOOST_ASSERT(repetition.from == 10);
    BOOST_ASSERT(repetition.to == 20);
}

BOOST_AUTO_TEST_CASE(RepetitionEmptyFirstArgument)
{
    auto text_iterator = TextIterator{"{, 1}"};
    text_iterator.next();

    DEBUG_VAR repetition = Repetition{text_iterator};

    BOOST_ASSERT(repetition.from == 0);
    BOOST_ASSERT(repetition.to == 1);
}

BOOST_AUTO_TEST_CASE(RepetitionFirstArgumentGreaterThanSecond)
{
    auto text_iterator = TextIterator{"{2, 1}"};
    text_iterator.next();

    BOOST_CHECK_EXCEPTION(
        Repetition{text_iterator},
        text::TextIteratorException,
        []([[maybe_unused]] const text::TextIteratorException &exception) {
            BOOST_ASSERT(exception.getColumn() == 1);
            BOOST_ASSERT(
                exception.getMessage() ==
                "the beginning of the repetition (2) is greater than the end (1)");
            return true;
        });
}

BOOST_AUTO_TEST_SUITE_END()

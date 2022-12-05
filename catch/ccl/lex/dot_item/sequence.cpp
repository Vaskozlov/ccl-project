#include <ccl/debug/debug_file.hpp>
#include <ccl/lex/dot_item/container.hpp>
#include <ccl/lex/dot_item/sequence.hpp>

using namespace ccl;
using namespace lex;
using namespace text;
using namespace dot_item;

// NOLINTNEXTLINE

BOOST_AUTO_TEST_SUITE(ContainerSequence)

BOOST_AUTO_TEST_CASE(SequenceWithOneCharBegin)
{
    auto text_iterator = TextIterator{R"("Hello, \"World\"!")"};
    text_iterator.next();

    auto string_item = Sequence({}, "\"", text_iterator);
    DEBUG_VAR &&string = string_item.getValue();

    BOOST_ASSERT(string == R"(Hello, "World"!)");
    BOOST_ASSERT(ccl::isEoF(text_iterator.next()));
}

BOOST_AUTO_TEST_CASE(SequenceWithTreeCharBegin)
{
    auto text_iterator = TextIterator{"\"\"\"Hello,\n    \"World\"!\"\"\""};
    text_iterator.next();

    auto string_item = Sequence({.multiline = true}, R"(""")", text_iterator);
    DEBUG_VAR &&string = string_item.getValue();

    BOOST_ASSERT(string == "Hello,\n    \"World\"!");
    BOOST_ASSERT(ccl::isEoF(text_iterator.next()));
}

BOOST_AUTO_TEST_CASE(UnterminatedSequence)
{
    auto text_iterator = TextIterator{R"("Hello, World!)"};
    text_iterator.next();

    BOOST_CHECK_EXCEPTION(
        Sequence({}, "\"", text_iterator),
        text::TextIteratorException,
        []([[maybe_unused]] const text::TextIteratorException &exception) {
            {
                BOOST_ASSERT(exception.getColumn() == 1);
                BOOST_ASSERT(exception.getMessage() == "unterminated sequence");
                return true;
            }
        });
}

BOOST_AUTO_TEST_CASE(SequenceReachedNewLine)
{
    auto text_iterator = TextIterator{"\"Hello, World!\n\""};
    text_iterator.next();

    BOOST_CHECK_EXCEPTION(
        Sequence({}, "\"", text_iterator),
        text::TextIteratorException,
        []([[maybe_unused]] const text::TextIteratorException &exception) {
            {
                BOOST_ASSERT(exception.getColumn() == 1);
                BOOST_ASSERT(
                    exception.getMessage() ==
                    "new line is reached, but sequence has not been terminated");
                BOOST_ASSERT(
                    exception.getSuggestion() == "use multiline sequence or close it with `\"`");
                return true;
            }
        });
}

BOOST_AUTO_TEST_SUITE_END()

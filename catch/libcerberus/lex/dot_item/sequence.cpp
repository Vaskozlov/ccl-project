#include <boost/test/unit_test.hpp>
#include <cerberus/lex/dot_item/sequence.hpp>

using namespace cerb::lex;
using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTNEXTLINE
static auto shared = AnalysisShared{};

BOOST_AUTO_TEST_SUITE(DotItemSequence)

BOOST_AUTO_TEST_CASE(SequenceWithOneCharBegin)
{
    auto text_iterator = TextIterator{ u8R"("Hello, \"World\"!")" };
    text_iterator.nextRawChar();

    auto string_item = Sequence({}, u8"\"", text_iterator, shared);
    const auto &string = string_item.get();

    BOOST_ASSERT(string == u8R"(Hello, "World"!)");
    BOOST_ASSERT(cerb::isEoF(text_iterator.nextRawChar()));
}

BOOST_AUTO_TEST_CASE(SequenceWithTreeCharBegin)
{
    auto text_iterator = TextIterator{ u8"\"\"\"Hello,\n    \"World\"!\"\"\"" };
    text_iterator.nextRawChar();

    auto string_item = Sequence({ .multiline = true }, u8R"(""")", text_iterator, shared);
    const auto &string = string_item.get();

    BOOST_ASSERT(string == u8"Hello,\n    \"World\"!");
    BOOST_ASSERT(cerb::isEoF(text_iterator.nextRawChar()));
}

BOOST_AUTO_TEST_CASE(UnterminatedSequence)
{
    auto text_iterator = TextIterator{ u8R"("Hello, World!)" };
    text_iterator.nextRawChar();

    BOOST_CHECK_EXCEPTION(
        Sequence({}, u8"\"", text_iterator, shared),
        SequenceException,
        [](const SequenceException &exception) {
            {
                BOOST_ASSERT(exception.getColumn() == 1);
                BOOST_ASSERT(exception.getMessage() == u8"unterminated sequence");
                return true;
            }
        });
}

BOOST_AUTO_TEST_CASE(SequenceReachedNewLine)
{
    auto text_iterator = TextIterator{ u8"\"Hello, World!\n\"" };
    text_iterator.nextRawChar();

    BOOST_CHECK_EXCEPTION(
        Sequence({}, u8"\"", text_iterator, shared),
        SequenceException,
        [](const SequenceException &exception) {
            {
                BOOST_ASSERT(exception.getColumn() == 1);
                BOOST_ASSERT(
                    exception.getMessage() ==
                    u8"new line is reached, but sequence has not been terminated");
                BOOST_ASSERT(
                    exception.getSuggestion() == u8"use multiline sequence or close it with `\"`");
                return true;
            }
        });
}

BOOST_AUTO_TEST_SUITE_END()

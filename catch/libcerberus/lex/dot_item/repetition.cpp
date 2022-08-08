#include <cerberus/debug/debug_file.hpp>
#include <cerberus/lex/dot_item/dot_item.hpp>

using namespace cerb::text;
using namespace cerb::lex::dot_item;

// NOLINTBEGIN

STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "{10, 20}" };
    text_iterator.nextRawChar();

    auto repetition = Repetition{ text_iterator };
    assertEqual(repetition.from, 10_ZU);
    assertEqual(repetition.to, 20_ZU);

    return {};
}
();

STRING_TEST
{
    auto text_iterator = TextIterator<char>{ "{, 1}" };
    text_iterator.nextRawChar();

    auto repetition = Repetition{ text_iterator };
    assertEqual(repetition.from, 0_ZU);
    assertEqual(repetition.to, 1_ZU);

    return {};
}
();

RUNTIME_TEST
{
    auto text_iterator = TextIterator<char>{ "{2, 1}" };
    text_iterator.nextRawChar();

    try {
        UNUSED_DECL Repetition{ text_iterator };
        assertTrue(false);
    } catch (const RepetitionException<char> &exception) {
        assertEqual(exception.getColumn(), 1_ZU);// NOLINT
        assertEqual(
            exception.getMessage(),
            "the beginning of the repetition (2) is greater than the end (1)");
    }

    return {};
}
();

// NOLINTEND

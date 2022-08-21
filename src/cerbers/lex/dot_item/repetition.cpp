#include <cerberus/lex/dot_item/repetition.hpp>

namespace cerb::lex::dot_item
{
    using namespace cerb::string_view_literals;

    Repetition::Repetition(text::TextIterator &text_iterator)
    {
        checkRangeStart(text_iterator);

        auto iterator_copy = text_iterator;

        from = parseNumber(text_iterator, U',');// NOLINT initialization via =
        to = parseNumber(text_iterator, U'}');  // NOLINT

        checkCorrectnessOfValues(iterator_copy);
    }

    auto Repetition::parseNumber(text::TextIterator &text_iterator, char32_t terminator) -> size_t
    {
        auto result = static_cast<size_t>(0);
        constexpr auto decimal_base = static_cast<size_t>(10);

        text_iterator.skipCommentsAndLayout();

        while (text_iterator.nextRawChar() != terminator) {
            auto chr = text_iterator.getCurrentChar();

            if (isDigit(chr)) {
                result = result * decimal_base + static_cast<size_t>(chr - U'0');
                continue;
            }

            throwUnexpectedCharacter(text_iterator, chr);
        }

        return result;
    }

    auto Repetition::checkRangeStart(text::TextIterator &text_iterator) -> void
    {
        if (text_iterator.getCurrentChar() != U'{') {
            throwRangeBeginException(text_iterator);
        }
    }

    auto Repetition::checkCorrectnessOfValues(text::TextIterator &text_iterator) const -> void
    {
        if (from > to) {
            throwBadValues(text_iterator);
        }
    }

    auto Repetition::throwBadValues(text::TextIterator &text_iterator) const -> void
    {
        auto message = fmt::format<
            u8"the beginning of the repetition ({}) is greater than the end "
            "({})">(from, to);

        text_iterator.throwException<RepetitionException>(message);
        throw UnrecoverableError{ "unrecoverable error in Repetition" };
    }

    auto Repetition::throwUnexpectedCharacter(text::TextIterator &text_iterator, char32_t chr)
        -> void
    {
        auto message = fmt::format<u8"expected a number, but found `{}`">(chr);

        text_iterator.throwException<RepetitionException>(message);
        throw UnrecoverableError{ "unrecoverable error in Repetition" };
    }

    auto Repetition::throwUnexpectedTermination(text::TextIterator &text_iterator) -> void
    {
        text_iterator.throwException<RepetitionException>(u8"unexpected termination"_sv);
        throw UnrecoverableError{ "unrecoverable error in Repetition" };
    }

    auto Repetition::throwRangeBeginException(text::TextIterator &text_iterator) -> void
    {
        text_iterator.throwException<RepetitionException>(
            u8"expected '{' at the beginning of repetition range"_sv);
        throw UnrecoverableError{ "unrecoverable error in Repetition" };
    }
}// namespace cerb::lex::dot_item

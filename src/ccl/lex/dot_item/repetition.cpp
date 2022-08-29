#include <ccl/lex/dot_item/recurrence.hpp>

namespace ccl::lex::dot_item
{
    using namespace ccl::string_view_literals;

    Recurrence::Recurrence(text::TextIterator &text_iterator)
    {
        checkRangeStart(text_iterator);

        auto iterator_copy = text_iterator;

        from = parseNumber(text_iterator, U',');// NOLINT initialization via =
        to = parseNumber(text_iterator, U'}');  // NOLINT

        checkCorrectnessOfValues(iterator_copy);
    }

    auto Recurrence::parseNumber(text::TextIterator &text_iterator, char32_t terminator) -> size_t
    {
        auto result = static_cast<size_t>(0);
        constexpr auto decimal_base = static_cast<size_t>(10);

        text_iterator.skipCommentsAndLayout();

        while (text_iterator.next() != terminator) {
            auto chr = text_iterator.getCurrentChar();

            if (isDigit(chr)) {
                result = result * decimal_base + static_cast<size_t>(chr - U'0');
                continue;
            }

            throwUnexpectedCharacter(text_iterator, chr);
        }

        return result;
    }

    auto Recurrence::checkRangeStart(text::TextIterator &text_iterator) -> void
    {
        if (text_iterator.getCurrentChar() != U'{') {
            throwRangeBeginException(text_iterator);
        }
    }

    auto Recurrence::checkCorrectnessOfValues(text::TextIterator &text_iterator) const -> void
    {
        if (from > to) {
            throwBadValues(text_iterator);
        }
    }

    auto Recurrence::throwBadValues(text::TextIterator &text_iterator) const -> void
    {
        auto message = ::fmt::format(
            "the beginning of the recurrence ({}) is greater than the end "
            "({})",
            from, to);

        text_iterator.throwError(message);
        throw UnrecoverableError{ "unrecoverable error in Recurrence" };
    }

    auto Recurrence::throwUnexpectedCharacter(text::TextIterator &text_iterator, char32_t chr)
        -> void
    {
        auto buffer = std::string{};
        utf8::appendUtf32ToUtf8Container(buffer, chr);

        auto message = ::fmt::format("expected a number, but found `{}`", buffer);

        text_iterator.throwError(message);
        throw UnrecoverableError{ "unrecoverable error in Recurrence" };
    }

    auto Recurrence::throwUnexpectedTermination(text::TextIterator &text_iterator) -> void
    {
        text_iterator.throwError("unexpected termination"_sv);
        throw UnrecoverableError{ "unrecoverable error in Recurrence" };
    }

    auto Recurrence::throwRangeBeginException(text::TextIterator &text_iterator) -> void
    {
        text_iterator.throwError("expected '{' at the beginning of recurrence range"_sv);
        throw UnrecoverableError{ "unrecoverable error in Recurrence" };
    }
}// namespace ccl::lex::dot_item

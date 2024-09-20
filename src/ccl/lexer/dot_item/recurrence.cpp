#include <ccl/lexer/rule/repetition.hpp>

namespace ccl::lexer::rule
{
    Repetition::Repetition(text::TextIterator &text_iterator)
    {
        checkRangeStart(text_iterator);
        const auto iterator_copy = text_iterator;

        from = parseNumber(text_iterator, ',');// NOLINT initialization via =
        to = parseNumber(text_iterator, '}');  // NOLINT

        checkCorrectnessOfValues(iterator_copy);
    }

    auto Repetition::parseNumber(text::TextIterator &text_iterator, const char32_t terminator)
        -> std::size_t
    {
        constexpr auto decimal_base = std::size_t{10};

        auto result = std::size_t{};
        text_iterator.moveToCleanChar();

        while (text_iterator.advance() != terminator) {
            const auto chr = text_iterator.getCurrentChar();

            if (isDigit(chr)) {
                result = result * decimal_base + static_cast<std::size_t>(chr - '0');
                continue;
            }

            throwUnexpectedCharacter(text_iterator, chr);
        }

        return result;
    }

    auto Repetition::checkRangeStart(const text::TextIterator &text_iterator) -> void
    {
        if (text_iterator.getCurrentChar() != '{') {
            throwRangeBeginException(text_iterator);
        }
    }

    auto Repetition::checkCorrectnessOfValues(const text::TextIterator &text_iterator) const -> void
    {
        if (from > to) {
            throwBadValues(text_iterator);
        }
    }

    auto Repetition::throwBadValues(const text::TextIterator &text_iterator) const -> void
    {
        text_iterator.throwCriticalError(
            AnalysisStage::LEXICAL_ANALYSIS,
            fmt::format(
                "the beginning of the repetition ({}) is greater than the end "
                "({})",
                from, to));
    }

    auto Repetition::throwUnexpectedCharacter(
        const text::TextIterator &text_iterator, const char32_t chr) -> void
    {
        auto buffer = std::string{};
        isl::utf8::appendUtf32ToUtf8Container(buffer, chr);

        text_iterator.throwPanicError(
            AnalysisStage::LEXICAL_ANALYSIS,
            fmt::format("expected a number, but found `{}`", buffer));

        throw UnrecoverableError{"unrecoverable error in Repetition"};
    }

    auto Repetition::throwRangeBeginException(const text::TextIterator &text_iterator) -> void
    {
        text_iterator.throwPanicError(
            AnalysisStage::LEXICAL_ANALYSIS, "expected '{' at the beginning of repetition range");

        throw UnrecoverableError{"unrecoverable error in Repetition"};
    }
}// namespace ccl::lexer::rule

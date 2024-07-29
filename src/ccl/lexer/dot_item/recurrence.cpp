#include <ccl/lexer/rule/closure.hpp>

namespace ccl::lexer::rule
{
    Closure::Closure(text::TextIterator &text_iterator)
    {
        checkRangeStart(text_iterator);
        auto iterator_copy = text_iterator;

        from = parseNumber(text_iterator, ',');// NOLINT initialization via =
        to = parseNumber(text_iterator, '}');  // NOLINT

        checkCorrectnessOfValues(iterator_copy);
    }

    auto Closure::parseNumber(text::TextIterator &text_iterator, char32_t terminator) -> std::size_t
    {
        constexpr auto decimal_base = isl::as<std::size_t>(10);

        auto result = isl::as<std::size_t>(0);
        text_iterator.moveToCleanChar();

        while (text_iterator.advance() != terminator) {
            const char32_t chr = text_iterator.getCurrentChar();

            if (isDigit(chr)) {
                result = result * decimal_base + isl::as<std::size_t>(chr - '0');
                continue;
            }

            throwUnexpectedCharacter(text_iterator, chr);
        }

        return result;
    }

    auto Closure::checkRangeStart(text::TextIterator &text_iterator) -> void
    {
        if (text_iterator.getCurrentChar() != '{') {
            throwRangeBeginException(text_iterator);
        }
    }

    auto Closure::checkCorrectnessOfValues(text::TextIterator &text_iterator) const -> void
    {
        if (from > to) {
            throwBadValues(text_iterator);
        }
    }

    auto Closure::throwBadValues(text::TextIterator &text_iterator) const -> void
    {
        auto message = fmt::format(
            "the beginning of the repetition ({}) is greater than the end "
            "({})",
            from, to);

        text_iterator.throwCriticalError(AnalysisStage::LEXICAL_ANALYSIS, message);
    }

    auto Closure::throwUnexpectedCharacter(text::TextIterator &text_iterator, char32_t chr) -> void
    {
        auto buffer = std::string{};
        isl::utf8::appendUtf32ToUtf8Container(buffer, chr);

        auto message = fmt::format("expected a number, but found `{}`", buffer);

        text_iterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, message);
        throw UnrecoverableError{"unrecoverable error in Repetition"};
    }

    auto Closure::throwRangeBeginException(text::TextIterator &text_iterator) -> void
    {
        text_iterator.throwPanicError(
            AnalysisStage::LEXICAL_ANALYSIS, "expected '{' at the beginning of repetition range");
        throw UnrecoverableError{"unrecoverable error in Repetition"};
    }
}// namespace ccl::lexer::rule

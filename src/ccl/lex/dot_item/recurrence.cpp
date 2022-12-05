#include <ccl/lex/dot_item/repetition.hpp>

namespace ccl::lex::dot_item
{
    using namespace text;
    using namespace ccl::string_view_literals;

    Repetition::Repetition(TextIterator &text_iterator)
    {
        checkRangeStart(text_iterator);
        auto iterator_copy = text_iterator;

        from = parseNumber(text_iterator, ',');// NOLINT initialization via =
        to = parseNumber(text_iterator, '}');  // NOLINT

        checkCorrectnessOfValues(iterator_copy);
    }

    auto Repetition::parseNumber(TextIterator &text_iterator, char32_t terminator) -> size_t
    {
        constexpr auto decimal_base = as<size_t>(10);

        auto result = as<size_t>(0);
        text_iterator.moveToCleanChar();

        while (text_iterator.next() != terminator) {
            auto chr = text_iterator.getCurrentChar();

            if (isDigit(chr)) {
                result = result * decimal_base + as<size_t>(chr - '0');
                continue;
            }

            throwUnexpectedCharacter(text_iterator, chr);
        }

        return result;
    }

    auto Repetition::checkRangeStart(TextIterator &text_iterator) -> void
    {
        if (text_iterator.getCurrentChar() != '{') {
            throwRangeBeginException(text_iterator);
        }
    }

    auto Repetition::checkCorrectnessOfValues(TextIterator &text_iterator) const -> void
    {
        if (from > to) {
            throwBadValues(text_iterator);
        }
    }

    auto Repetition::throwBadValues(TextIterator &text_iterator) const -> void
    {
        auto message = fmt::format(
            "the beginning of the repetition ({}) is greater than the end "
            "({})",
            from, to);

        text_iterator.throwCriticalError(AnalysisStage::LEXICAL_ANALYSIS, message);
    }

    auto Repetition::throwUnexpectedCharacter(TextIterator &text_iterator, char32_t chr) -> void
    {
        auto buffer = std::string{};
        utf8::appendUtf32ToUtf8Container(buffer, chr);

        auto message = fmt::format("expected a number, but found `{}`", buffer);

        text_iterator.throwPanicError(AnalysisStage::LEXICAL_ANALYSIS, message);
        throw UnrecoverableError{"unrecoverable error in Repetition"};
    }

    auto Repetition::throwRangeBeginException(TextIterator &text_iterator) -> void
    {
        text_iterator.throwPanicError(
            AnalysisStage::LEXICAL_ANALYSIS,
            "expected '{' at the beginning of repetition range"_sv);
        throw UnrecoverableError{"unrecoverable error in Repetition"};
    }
}// namespace ccl::lex::dot_item

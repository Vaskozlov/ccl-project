#include <ccl/text/text_iterator.hpp>

namespace ccl::text
{
    using NotationEscapingSymbolizer = TextIterator::NotationEscapingSymbolizer;

    NotationEscapingSymbolizer::NotationEscapingSymbolizer(
        TextIterator &text_iterator,
        u16 maximum_symbols,
        u16 notation_power,
        bool are_all_chars_required)
      : textIterator{text_iterator}
      , maximumSymbols{maximum_symbols}
      , notationPower{notation_power}
      , areAllCharsRequired{are_all_chars_required}
    {
        checkNotation();
        calculateResult();
    }

    auto NotationEscapingSymbolizer::calculateResult() -> void
    {
        auto chars_count = as<size_t>(0);

        for (; chars_count != maximumSymbols; ++chars_count) {
            auto chr = textIterator.futureChar(1);

            if (isEoF(chr) || isOutOfNotation(chr)) [[unlikely]] {
                break;
            }

            result = as<char32_t>(result << notationPower);
            result += as<char32_t>(HexadecimalCharsToInt<char32_t>.at(chr));

            textIterator.next();
        }

        checkAllCharsUsage(chars_count);
    }

    auto NotationEscapingSymbolizer::createSuggestionNotEnoughChars(size_t chars_count) const
        -> std::string
    {
        auto suggestion_message = as<std::string>(textIterator.getWorkingLine());
        insertExtraZerosToNotEnoughMessage(chars_count, suggestion_message);
        return suggestion_message;
    }

    auto NotationEscapingSymbolizer::insertExtraZerosToNotEnoughMessage(
        size_t chars_count,
        std::string &message) const -> void
    {
        auto column = textIterator.getColumn();
        auto insertion_size = as<size_t>(maximumSymbols - chars_count);
        auto insertion_position = column - chars_count;

        message.insert(insertion_position, insertion_size, '0');
    }

    auto NotationEscapingSymbolizer::checkNotation() const -> void
    {
        CCL_ASSERT(land(notationPower > 0, notationPower <= 4));
    }

    auto NotationEscapingSymbolizer::isOutOfNotation(char32_t chr) const -> bool
    {
        return !HexadecimalCharsToInt<char32_t>.contains(chr) ||
               HexadecimalCharsToInt<char32_t>.at(chr) >= pow2(notationPower);
    }

    auto NotationEscapingSymbolizer::checkAllCharsUsage(size_t chars_count) const -> void
    {
        if (land(areAllCharsRequired, chars_count != maximumSymbols)) {
            throwNotEnoughCharsException(chars_count);
        }
    }

    auto NotationEscapingSymbolizer::throwNotEnoughCharsException(size_t chars_count) const -> void
    {
        auto exception_message = fmt::format(
            "expected {} characters, but only {} of them were provided", maximumSymbols,
            chars_count);

        auto suggestion_message = createSuggestionNotEnoughChars(chars_count);

        textIterator.throwUncriticalError(
            AnalysisStage::LEXICAL_ANALYSIS, exception_message, suggestion_message);
    }
}// namespace ccl::text

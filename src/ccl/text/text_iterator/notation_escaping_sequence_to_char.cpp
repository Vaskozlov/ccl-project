#include <ccl/text/text_iterator.hpp>

namespace ccl::text
{
    using NotationEscapingSequenceToChar = TextIterator::NotationEscapingSequenceToChar;

    NotationEscapingSequenceToChar::NotationEscapingSequenceToChar(
        TextIterator &text_iterator,
        const u16 maximum_symbols,
        const u16 notation_power,
        const bool are_all_chars_required)
      : textIterator{text_iterator}
      , maximumSymbols{maximum_symbols}
      , notationPower{notation_power}
      , areAllCharsRequired{are_all_chars_required}
    {
        checkNotation();
        calculateResult();
    }

    auto NotationEscapingSequenceToChar::calculateResult() -> void
    {
        auto chars_count = u32{};

        for (; chars_count != maximumSymbols; ++chars_count) {
            auto chr = textIterator.futureChar();

            if (isEoF(chr) || isOutOfNotation(chr)) {
                break;
            }

            result = static_cast<char32_t>(result << notationPower);
            result += static_cast<char32_t>(HexadecimalCharsToInt.at(chr));

            textIterator.advance();
        }

        checkAllCharsUsage(chars_count);
    }

    auto NotationEscapingSequenceToChar::createSuggestionNotEnoughChars(const u32 chars_count) const
        -> std::string
    {
        auto suggestion_message = isl::as<std::string>(textIterator.getCurrentLine());
        insertExtraZerosToNotEnoughMessage(chars_count, suggestion_message);
        return suggestion_message;
    }

    auto NotationEscapingSequenceToChar::insertExtraZerosToNotEnoughMessage(
        const u32 chars_count,
        std::string &message) const -> void
    {
        const auto column = textIterator.getColumn();
        const auto insertion_size = isl::as<u32>(maximumSymbols - chars_count);
        const auto insertion_position = column - chars_count;

        message.insert(insertion_position, insertion_size, '0');
    }

    auto NotationEscapingSequenceToChar::checkNotation() const -> void
    {
        ISL_ASSERT(notationPower > 0 && notationPower <= 4);
    }

    auto NotationEscapingSequenceToChar::isOutOfNotation(const char32_t chr) const -> bool
    {
        return !HexadecimalCharsToInt.contains(chr) ||
               HexadecimalCharsToInt.at(chr) >= (1U << notationPower);
    }

    auto NotationEscapingSequenceToChar::checkAllCharsUsage(const u32 chars_count) const -> void
    {
        if (areAllCharsRequired && chars_count != maximumSymbols) {
            throwNotEnoughCharsException(chars_count);
        }
    }

    auto NotationEscapingSequenceToChar::throwNotEnoughCharsException(const u32 chars_count) const
        -> void
    {
        textIterator.throwUncriticalError(
            AnalysisStage::LEXICAL_ANALYSIS,
            fmt::format(
                "expected {} characters, but only {} of them were provided", maximumSymbols,
                chars_count),
            createSuggestionNotEnoughChars(chars_count));
    }
}// namespace ccl::text

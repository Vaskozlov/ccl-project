#include <ccl/text/text_iterator.hpp>

namespace ccl::text
{
    auto TextIterator::NotationEscapingSymbolizer::calculateResult() -> void
    {
        u16 chars_count = 0;

        for (; chars_count != max_times; ++chars_count) {
            auto chr = text_iterator.next();

            if (lor(isEoF(chr), isOutOfNotation(chr))) {
                break;
            }

            result = as<char32_t>(result << notation_power);
            result += as<char32_t>(HexadecimalCharsToInt<char32_t>.at(chr));
        }

        checkAllCharsUsage(chars_count);
    }

    auto TextIterator::NotationEscapingSymbolizer::createSuggestionNotEnoughChars(
        u16 chars_count) const -> std::string
    {
        auto suggestion_message = as<std::string>(text_iterator.getWorkingLine());
        insertExtraZerosToNotEnoughMessage(chars_count, suggestion_message);
        return suggestion_message;
    }

    auto TextIterator::NotationEscapingSymbolizer::insertExtraZerosToNotEnoughMessage(
        u16 chars_count,
        std::string &message) const -> void
    {
        auto column = text_iterator.getColumn();
        auto insertion_size = as<size_t>(max_times - chars_count);
        auto insertion_position = column - chars_count;

        message.insert(insertion_position, insertion_size, '0');
    }

    auto TextIterator::NotationEscapingSymbolizer::checkNotation() const -> void
    {
        CCL_ASSERT(land(notation_power > 0, notation_power <= 4));
    }

    auto TextIterator::NotationEscapingSymbolizer::isOutOfNotation(char32_t chr) const -> bool
    {
        return not HexadecimalCharsToInt<char32_t>.contains(chr) ||
               HexadecimalCharsToInt<char32_t>.at(chr) >= pow2(notation_power);
    }

    auto TextIterator::NotationEscapingSymbolizer::checkAllCharsUsage(u16 chars_count) const -> void
    {
        if (land(need_all_chars, chars_count != max_times)) {
            throwNotEnoughCharsException(chars_count);
        }
    }

    auto TextIterator::NotationEscapingSymbolizer::throwNotEnoughCharsException(
        u16 chars_count) const -> void
    {
        auto exception_message = fmt::format(
            "expected {} characters, but only {} of them were provided", max_times, chars_count);

        auto suggestion_message = createSuggestionNotEnoughChars(chars_count);

        text_iterator.throwUncriticalError(
            AnalysationStage::LEXICAL_ANALYSIS, exception_message, suggestion_message);
    }

    TextIterator::NotationEscapingSymbolizer::NotationEscapingSymbolizer(
        TextIterator &text_iterator_,
        u16 max_times_,
        u16 notation_power_,
        bool need_all_chars_)
      : text_iterator{ text_iterator_ }, max_times{ max_times_ }, notation_power{ notation_power_ },
        need_all_chars{ need_all_chars_ }
    {
        checkNotation();
        calculateResult();
    }
}// namespace ccl::text

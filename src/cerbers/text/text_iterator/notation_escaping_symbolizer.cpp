#include <cerberus/text/text_iterator.hpp>

namespace cerb::text
{
    auto TextIterator::NotationEscapingSymbolizer::calculateResult() -> void
    {
        u16 chars_count = 0;

        for (; chars_count != max_times; ++chars_count) {
            auto chr = text_iterator.nextRawChar();

            if (lor(isEoF(chr), isOutOfNotation(chr))) {
                break;
            }

            checkCharacterOverflow();

            result = static_cast<char32_t>(result << notation_power);
            result += static_cast<char32_t>(HexadecimalCharsToInt<char32_t>.at(chr));
        }

        checkAllCharsUsage(chars_count);
    }

    auto TextIterator::NotationEscapingSymbolizer::createSuggestionNotEnoughChars(
        u16 chars_count) const -> std::u8string
    {
        auto suggestion_message = static_cast<std::u8string>(text_iterator.getWorkingLine());
        insertExtraZerosToNotEnoughMessage(chars_count, suggestion_message);
        return suggestion_message;
    }

    auto TextIterator::NotationEscapingSymbolizer::insertExtraZerosToNotEnoughMessage(
        u16 chars_count,
        std::u8string &message) const -> void
    {
        auto column = text_iterator.getColumn();
        auto insertion_size = static_cast<size_t>(max_times - chars_count);
        auto insertion_position = column - chars_count;

        message.insert(insertion_position, insertion_size, u8'0');
    }

    auto TextIterator::NotationEscapingSymbolizer::checkNotation() const -> void
    {
        assert(land(notation_power > 0, notation_power <= 4));
    }

    auto TextIterator::NotationEscapingSymbolizer::isOutOfNotation(char32_t chr) const -> bool
    {
        return not HexadecimalCharsToInt<char32_t>.contains(chr) ||
               HexadecimalCharsToInt<char32_t>.at(chr) >= (1U << notation_power);
    }

    auto TextIterator::NotationEscapingSymbolizer::checkCharacterOverflow() const -> void
    {
        constexpr auto size_in_bits = sizeof(char32_t) * 8;

        if ((result >> (size_in_bits - notation_power)) != 0) {
            throwCharacterOverflow();
        }
    }

    auto TextIterator::NotationEscapingSymbolizer::checkAllCharsUsage(u16 chars_count) const -> void
    {
        if (land(need_all_chars, chars_count != max_times)) {
            throwNotEnoughCharsException(chars_count);
        }
    }

    auto TextIterator::NotationEscapingSymbolizer::throwCharacterOverflow() const -> void
    {
        using namespace std::string_view_literals;

        text_iterator.template throwException<NotationEscapingSymbolizerException>(
            u8"character literal overflow"sv);
    }

    auto TextIterator::NotationEscapingSymbolizer::throwNotEnoughCharsException(
        u16 chars_count) const -> void
    {
        auto exception_message =
            fmt::format<u8"expected {} characters, but only {} of them were provided">(
                max_times, chars_count);

        auto suggestion_message = createSuggestionNotEnoughChars(chars_count);

        text_iterator.template throwException<NotationEscapingSymbolizerException>(
            exception_message, suggestion_message);
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
}// namespace cerb::text

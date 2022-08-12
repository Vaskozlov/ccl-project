#ifndef CERBERUS_PROJECT_NOTATION_ESCAPING_SYMBOL_HPP
#define CERBERUS_PROJECT_NOTATION_ESCAPING_SYMBOL_HPP

#include <cerberus/text/text_iterator_modules/exception.hpp>

namespace cerb::text::module
{
    template<CharacterLiteral CharT>
    CERBLIB_EXCEPTION(NotationEscapingSymbolizerException, TextIteratorException<CharT>);

    template<CharacterLiteral TextT, CharacterLiteral EscapingT>
    class NotationEscapingSymbolizer
    {
    private:
        using text_iterator_t = TextIterator<TextT, EscapingT>;

    public:
        constexpr auto get() const -> EscapingT
        {
            return result;
        }

        auto operator=(NotationEscapingSymbolizer &&) -> void = delete;
        auto operator=(const NotationEscapingSymbolizer &) -> void = delete;

        NotationEscapingSymbolizer() = delete;
        NotationEscapingSymbolizer(NotationEscapingSymbolizer &&) = delete;
        NotationEscapingSymbolizer(const NotationEscapingSymbolizer &) = delete;

        constexpr NotationEscapingSymbolizer(
            text_iterator_t &text_iterator_, u16 max_times_, u16 notation_power_,
            bool need_all_chars_)
          : text_iterator{ text_iterator_ }, max_times{ max_times_ },
            notation_power{ notation_power_ }, need_all_chars{ need_all_chars_ }
        {
            checkNotation();
            calculateResult();
        }

        ~NotationEscapingSymbolizer() = default;

    private:
        constexpr auto calculateResult() -> void
        {
            u16 chars_count = 0;

            for (; chars_count != max_times; ++chars_count) {
                auto chr = text_iterator.nextRawChar();

                if (lor(isEoF(chr), isOutOfNotation(chr))) {
                    break;
                }

                checkCharacterOverflow();

                result = static_cast<EscapingT>(result << notation_power);
                result += static_cast<EscapingT>(HexadecimalCharsToInt<TextT>.at(chr));
            }

            checkAllCharsUsage(chars_count);
        }

        constexpr auto checkNotation() const -> void
        {
            constexpr auto max_notation_power = 4;

            if (lor(notation_power == 0, notation_power > max_notation_power)) {
                throw LogicError("notation power must be between 1 and 4");
            }
        }

        constexpr auto isOutOfNotation(TextT chr) const -> bool
        {
            return not HexadecimalCharsToInt<TextT>.contains(chr) ||
                   HexadecimalCharsToInt<TextT>.at(chr) >= (1U << notation_power);
        }

        constexpr auto checkCharacterOverflow() const -> void
        {
            constexpr auto size_in_bits = sizeof(EscapingT) * 8;

            if ((result >> (size_in_bits - notation_power)) != 0) {
                throwCharacterOverflow();
            }
        }

        constexpr auto throwCharacterOverflow() const -> void
        {
            text_iterator.throwException(NotationEscapingSymbolizerException<TextT>{
                text_iterator, "character literal overflow" });
        }

        constexpr auto checkAllCharsUsage(u16 chars_count) const -> void
        {
            if (land(need_all_chars, chars_count != max_times)) {
                throwNotEnoughCharsException(chars_count);
            }
        }

        constexpr auto throwNotEnoughCharsException(u16 chars_count) const -> void
        {
            auto exception_message =
                fmt::format<TextT, "expected {} characters, but only {} of them were provided">(
                    max_times, chars_count);

            auto suggestion_message = createSuggestionNotEnoughChars(chars_count);

            text_iterator.throwException(NotationEscapingSymbolizerException<TextT>{
                text_iterator, exception_message, suggestion_message });
        }

        constexpr auto createSuggestionNotEnoughChars(u16 chars_count) const -> Str<TextT>
        {
            auto suggestion_message = static_cast<Str<TextT>>(text_iterator.getWorkingLine());

            insertExtraZerosToNotEnoughMessage(chars_count, suggestion_message);

            return suggestion_message;
        }

        constexpr auto
            insertExtraZerosToNotEnoughMessage(u16 chars_count, Str<TextT> &message) const -> void
        {
            auto column = text_iterator.getColumn();
            auto insertion_size = static_cast<size_t>(max_times - chars_count);
            auto insertion_position = column - chars_count;

            message.insert(insertion_position, insertion_size, '0');
        }

        text_iterator_t &text_iterator;
        u16 max_times;
        u16 notation_power;
        EscapingT result{};
        bool need_all_chars;
    };

    template<CharacterLiteral TextT, CharacterLiteral EscapingT>
    constexpr auto calculateNotationEscapeSymbol(
        TextIterator<TextT, EscapingT> &text_iterator, u16 max_times, u16 notation_power,
        bool need_all_chars) -> EscapingT
    {
        auto notation_escape_symbolizer =
            NotationEscapingSymbolizer<TextT, EscapingT>{ text_iterator, max_times, notation_power,
                                                          need_all_chars };
        return notation_escape_symbolizer.get();
    }
}// namespace cerb::text::module

#endif /* CERBERUS_PROJECT_NOTATION_ESCAPING_SYMBOL_HPP */

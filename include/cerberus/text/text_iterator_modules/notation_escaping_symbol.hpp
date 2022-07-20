#ifndef CERBERUS_PROJECT_NOTATION_ESCAPING_SYMBOL_HPP
#define CERBERUS_PROJECT_NOTATION_ESCAPING_SYMBOL_HPP

#include <cerberus/text/text_iterator_modules/exception.hpp>

namespace cerb::text::module
{
    template<CharacterLiteral CharT>
    CERBLIB_EXCEPTION(NotationEscapingSymbolizerException, TextIteratorException<CharT>);

    template<CharacterLiteral CharT>
    class NotationEscapingSymbolizer
    {
    private:
        using text_iterator_t = TextIterator<CharT>;

    public:
        constexpr auto get() const -> CharT
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

                result = static_cast<CharT>(result << notation_power);
                result += static_cast<CharT>(HexadecimalCharsToInt<CharT>.at(chr));
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

        constexpr auto isOutOfNotation(CharT chr) const -> bool
        {
            return not HexadecimalCharsToInt<CharT>.contains(chr) ||
                   HexadecimalCharsToInt<CharT>.at(chr) >= (1U << notation_power);
        }

        constexpr auto checkCharacterOverflow() const -> void
        {
            constexpr auto size_in_bits = sizeof(CharT) * 8;

            if ((result >> (size_in_bits - notation_power)) != 0) {
                throwCharacterOverflow();
            }
        }

        constexpr auto throwCharacterOverflow() const -> void
        {
            auto exception = NotationEscapingSymbolizerException<CharT>(
                text_iterator, "character literal overflow");
            text_iterator.throwException(std::move(exception));
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
                fmt::format<CharT, "expected {} characters, but only {} of them were provided">(
                    max_times, chars_count);

            auto exception = NotationEscapingSymbolizerException<CharT>{
                text_iterator, exception_message, createSuggestionNotEnoughChars(chars_count)
            };

            text_iterator.throwException(std::move(exception));
        }

        constexpr auto createSuggestionNotEnoughChars(u16 chars_count) const -> Str<CharT>
        {
            auto suggestion_message = static_cast<Str<CharT>>(text_iterator.getWorkingLine());

            insertExtraZerosToNotEnoughMessage(chars_count, suggestion_message);

            return suggestion_message;
        }

        constexpr auto
            insertExtraZerosToNotEnoughMessage(u16 chars_count, Str<CharT> &message) const -> void
        {
            auto column = text_iterator.getColumn();
            message.insert(column - chars_count, max_times - chars_count, '0');
        }

        text_iterator_t &text_iterator;
        u16 max_times;
        u16 notation_power;
        CharT result{};
        bool need_all_chars;
    };

    template<CharacterLiteral CharT>
    constexpr auto calculateNotationEscapeSymbol(
        TextIterator<CharT> &text_iterator, u16 max_times, u16 notation_power, bool need_all_chars)
        -> CharT
    {
        auto notation_escape_symbolizer =
            NotationEscapingSymbolizer{ text_iterator, max_times, notation_power, need_all_chars };
        return notation_escape_symbolizer.get();
    }
}// namespace cerb::text::module

#endif /* CERBERUS_PROJECT_NOTATION_ESCAPING_SYMBOL_HPP */

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
            text_iterator_t &text_iterator_, u16 max_times_, u16 notation_power_)
          : text_iterator{ text_iterator_ }, max_times{ max_times_ }, notation_power{
                notation_power_
            }
        {
            checkNotation();
            calculateResult();
        }

        ~NotationEscapingSymbolizer() = default;

    private:
        constexpr auto calculateResult() -> void
        {
            for (u16 i = 0; i != max_times; ++i) {
                auto chr = text_iterator.nextRawChar();

                if (lor(isEoF(chr), isOutOfNotation(chr))) {
                    break;
                }

                checkCharacterOverflow();

                result = static_cast<CharT>(result << notation_power);
                result += static_cast<CharT>(HexadecimalCharsToInt<CharT>.at(chr));
            }
        }

        constexpr auto checkNotation() const -> void
        {
            constexpr auto max_notation_power = 4;

            if (lor(notation_power == 0, notation_power > max_notation_power)) {
                throw LogicError{ "notation power must be between 1 and 4" };
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
                throw NotationEscapingSymbolizerException<CharT>{ text_iterator,
                                                                  "character literal overflow" };
            }
        }

        text_iterator_t &text_iterator;
        u16 max_times;
        u16 notation_power;
        CharT result{};
    };

    template<CharacterLiteral CharT>
    constexpr auto calculateNotationEscapeSymbol(
        TextIterator<CharT> &text_iterator, u16 max_times, u16 notation_power) -> CharT
    {
        auto notation_escape_symbolizer =
            NotationEscapingSymbolizer{ text_iterator, max_times, notation_power };
        return notation_escape_symbolizer.get();
    }
}// namespace cerb::text::module

#endif /* CERBERUS_PROJECT_NOTATION_ESCAPING_SYMBOL_HPP */

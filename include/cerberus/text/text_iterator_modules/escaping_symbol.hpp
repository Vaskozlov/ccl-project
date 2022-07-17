#ifndef CERBERUS_PROJECT_ESCAPING_SYMBOL_HPP
#define CERBERUS_PROJECT_ESCAPING_SYMBOL_HPP

#include <cerberus/text/text_iterator_modules/notation_escaping_symbol.hpp>

namespace cerb::text::module
{
    template<CharacterLiteral CharT>
    class TextIterator;

    template<CharacterLiteral CharT>
    class EscapingSymbolizer
    {
        using text_iterator_t = TextIterator<CharT>;
        using extra_symbols_t = std::initializer_list<std::pair<CharT, CharT>>;

    public:
        constexpr auto match() -> CharT
        {
            auto chr = text_iterator.nextRawChar();

            switch (chr) {
            case '\\':
                return '\\';

            case '\'':
                return '\'';

            case '\"':
                return '\"';

            case 'n':
                return '\n';

            case 't':
                return '\t';

            case 'r':
                return '\r';

            case 'f':
                return '\f';

            case 'b':
                return '\b';

            case '0':
                return calculateNotationEscapeSymbol(text_iterator, 2, 3);

            case 'x':
                return calculateNotationEscapeSymbol(text_iterator, 2, 4);

            case 'u':
                return calculateNotationEscapeSymbol(text_iterator, 4, 4);

            default:
                return tryExtraSymbolsMatch(chr);
            }
        }

        auto operator=(EscapingSymbolizer &&) -> void = delete;
        auto operator=(const EscapingSymbolizer &) -> void = delete;

        EscapingSymbolizer() = delete;
        EscapingSymbolizer(EscapingSymbolizer &&) = delete;
        EscapingSymbolizer(const EscapingSymbolizer &) = delete;

        constexpr EscapingSymbolizer(
            text_iterator_t text_iterator_,
            const extra_symbols_t &extra_symbols_)
          : text_iterator(text_iterator_), extra_symbols(extra_symbols_)
        {}

        ~EscapingSymbolizer() = default;

    private:
        constexpr auto tryExtraSymbolsMatch(CharT chr) -> CharT
        {
            auto it = std::find_if(extra_symbols.begin(), extra_symbols.end(), [chr](auto elem) {
                return elem.first == chr;
            });

            if (it == extra_symbols.end()) {
                throw TextIteratorException{ text_iterator, "unable to match any escaping symbol" };
            }

            return it->second;
        }

        text_iterator_t &text_iterator;
        const extra_symbols_t &extra_symbols;
    };
}// namespace cerb::text::module

#endif /* CERBERUS_PROJECT_ESCAPING_SYMBOL_HPP */

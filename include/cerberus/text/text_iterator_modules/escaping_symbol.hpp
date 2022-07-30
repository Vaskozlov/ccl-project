#ifndef CERBERUS_PROJECT_ESCAPING_SYMBOL_HPP
#define CERBERUS_PROJECT_ESCAPING_SYMBOL_HPP

#include <cerberus/text/text_iterator_modules/notation_escaping_symbol.hpp>

namespace cerb::text::module
{
    template<CharacterLiteral TextT, CharacterLiteral EscapingT = TextT>
    class EscapingSymbolizer
    {
    private:
        using text_iterator_t = TextIterator<TextT, EscapingT>;

    public:
        using extra_symbols_t = std::vector<std::pair<TextT, EscapingT>>;

        CERBLIB_DECL auto getExtraSymbols() const -> const extra_symbols_t &
        {
            return extra_symbols;
        }

        constexpr auto setExtraSymbols(extra_symbols_t &&extra_symbols_) -> void
        {
            extra_symbols = std::move(extra_symbols_);
        }

        constexpr auto setExtraSymbols(const extra_symbols_t &extra_symbols_) -> void
        {
            extra_symbols = extra_symbols_;
        }

        CERBLIB_DECL auto match() -> EscapingT
        {
            if (text_iterator.getCurrentChar() != '\\') {
                throw LogicError("called EscapingSymbolizer::match() without preceding '\\'");
            }

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

            case 'v':
                return '\v';

            case 'a':
                return '\a';

            case '0':
                return calculateNotationEscapeSymbol(text_iterator, 2, 3, false);

            case 'x':
                return calculateNotationEscapeSymbol(text_iterator, 2, 4, true);

            case 'u':
                return calculateNotationEscapeSymbol(text_iterator, 4, 4, true);

            case 'U':
                return calculateNotationEscapeSymbol(text_iterator, 8, 4, true);// NOLINT

            default:
                return searchInExtraSymbols(chr);
            }
        }

        auto operator=(EscapingSymbolizer &&) -> void = delete;
        auto operator=(const EscapingSymbolizer &) -> void = delete;

        EscapingSymbolizer() = delete;
        EscapingSymbolizer(EscapingSymbolizer &&) = delete;
        EscapingSymbolizer(const EscapingSymbolizer &) = delete;

        constexpr explicit EscapingSymbolizer(
            text_iterator_t &text_iterator_,
            const extra_symbols_t &extra_symbols_)
          : text_iterator(text_iterator_), extra_symbols(extra_symbols_)
        {}

        ~EscapingSymbolizer() = default;

    private:
        constexpr auto searchInExtraSymbols(TextT chr) -> EscapingT
        {
            auto it =
                std::ranges::find_if(extra_symbols, [chr](auto elem) { return elem.first == chr; });

            if (it == extra_symbols.end()) {
                throwMatchException();
                return '?';
            }

            return it->second;
        }

        constexpr auto throwMatchException() -> void
        {
            text_iterator.throwException(
                TextIteratorException{ text_iterator, "unable to match any escaping symbol" });
        }

        text_iterator_t &text_iterator;
        extra_symbols_t extra_symbols;
    };

    template<CharacterLiteral TextT, CharacterLiteral EscapingT>
    CERBLIB_DECL auto doEscapeSymbolizing(
        TextIterator<TextT, EscapingT> &text_iterator,
        const std::vector<std::pair<TextT, EscapingT>> &extra_symbols_) -> EscapingT
    {
        auto symbolizer = EscapingSymbolizer<TextT, EscapingT>{ text_iterator, extra_symbols_ };
        return symbolizer.match();
    }
}// namespace cerb::text::module

#endif /* CERBERUS_PROJECT_ESCAPING_SYMBOL_HPP */

#include <ccl/text/text_iterator.hpp>

namespace ccl::text
{
    auto TextIterator::EscapingSymbolizer::matchNextChar() -> char32_t
    {
        if (text_iterator.getCurrentChar() != '\\') {
            throw LogicError("called EscapingSymbolizer::matchNextChar() without preceding `\\`");
        }

        auto chr = text_iterator.next();

        switch (chr) {
        case U'\\':
            return U'\\';

        case U'\'':
            return U'\'';

        case U'\"':
            return U'\"';

        case U'n':
            return U'\n';

        case U't':
            return U'\t';

        case U'r':
            return U'\r';

        case U'f':
            return U'\f';

        case U'b':
            return U'\b';

        case U'v':
            return U'\v';

        case U'a':
            return U'\a';

        case U'0':
            return calculateNotationEscapeSymbol(text_iterator, 2, 3, false);

        case U'x':
            return calculateNotationEscapeSymbol(text_iterator, 2, 4, true);

        case U'u':
            return calculateNotationEscapeSymbol(text_iterator, 4, 4, true);

        case U'U':
            return calculateNotationEscapeSymbol(text_iterator, 8, 4, true);// NOLINT

        default:
            return searchInExtraSymbols(chr);
        }
    }

    auto TextIterator::EscapingSymbolizer::searchInExtraSymbols(char32_t chr) -> char32_t
    {
        auto it =
            std::ranges::find_if(extra_symbols, [chr](auto elem) { return elem.first == chr; });

        if (it == extra_symbols.end()) {
            throwMatchException();
            return U'?';
        }

        return it->second;
    }

    auto TextIterator::EscapingSymbolizer::throwMatchException() -> void
    {
        text_iterator.throwError<TextIteratorException>(
            "unable to matchNextChar any escaping symbol");
    }
}// namespace ccl::text

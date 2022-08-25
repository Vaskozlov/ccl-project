#include <ccl/text/text_iterator.hpp>

namespace ccl::text
{
    auto TextIterator::onMove(char8_t chr) -> void
    {
        location.intermediateNext(chr);
    }

    auto TextIterator::onCharacter(char32_t chr) -> void
    {
        location.next(chr);
        ts_tracker.next(chr);
        line_tracker.next(chr);
    }

    auto TextIterator::onUtfError(char8_t /* chr */) -> void
    {
        throwException<TextIteratorException>(u8"invalid utf symbol");
        throw UnrecoverableError{ "unable to recover, because of invalid utf symbol" };
    }

    auto TextIterator::skipComments() -> bool
    {
        return CommentSkipper{ *this, comment_tokens }.skip();
    }

    auto TextIterator::skipCommentsAndLayout() -> void
    {
        auto comment_skipper = CommentSkipper{ *this, comment_tokens };

        do {
            Base::moveToCleanChar();
        } while (comment_skipper.skip());
    }

    auto TextIterator::nextRawCharWithEscapingSymbols(
        TextIterator::extra_symbols_t const &extra_symbols) -> Pair<bool, char32_t>
    {
        auto escaping = false;
        auto chr = next();

        if (chr == U'\\') {
            escaping = true;
            chr = doEscapeSymbolizing(*this, extra_symbols);
        }

        Base::setCurrentChar(chr);
        return { escaping, chr };
    }

    auto TextIterator::doEscapeSymbolizing(
        TextIterator &text_iterator, TextIterator::extra_symbols_t const &extra_symbols_)
        -> char32_t
    {
        auto symbolizer = EscapingSymbolizer{ text_iterator, extra_symbols_ };
        return symbolizer.match();
    }

    auto TextIterator::calculateNotationEscapeSymbol(
        TextIterator &text_iterator, u16 max_times, u16 notation_power, bool need_all_chars)
        -> char32_t
    {
        auto notation_escape_symbolizer =
            NotationEscapingSymbolizer{ text_iterator, max_times, notation_power, need_all_chars };
        return notation_escape_symbolizer.get();
    }
}// namespace ccl::text

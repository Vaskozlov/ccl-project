#include <ccl/text/text_iterator.hpp>

namespace ccl::text
{
    TextIterator::TextIterator(
        isl::string_view input,
        ExceptionHandler &exception_handler,
        isl::string_view filename)
      : CrtpBasicTextIterator{input}
      , location{filename}
      , lineTracker{input}
      , exceptionHandler{&exception_handler}
    {}

    TextIterator::TextIterator(
        isl::string_view input,
        ExceptionHandler &exception_handler,
        const Location &iterator_location)
      : CrtpBasicTextIterator{input}
      , location{iterator_location}
      , lineTracker{input}
      , exceptionHandler{&exception_handler}
    {}

    auto TextIterator::nextCharWithEscapingSymbols(const extra_symbols_t &extra_symbols)
        -> isl::Pair<bool, char32_t>
    {
        auto escaping = false;
        char32_t chr = advance();

        if (U'\\' == chr) {
            escaping = true;
            chr = doEscapeSymbolizing(*this, extra_symbols);
        }

        CrtpBasicTextIterator::setCurrentChar(chr);
        return {escaping, chr};
    }

    auto TextIterator::doEscapeSymbolizing(
        TextIterator &text_iterator,
        const extra_symbols_t &extra_symbols) -> char32_t
    {
        return EscapingSequenceToChar(text_iterator, extra_symbols).matchNextChar();
    }

    auto TextIterator::calculateNotationEscapeSymbol(
        TextIterator &text_iterator,
        u16 max_times,
        u16 notation_power,
        bool need_all_chars) -> char32_t
    {
        return NotationEscapingSequenceToChar(
                   text_iterator, max_times, notation_power, need_all_chars)
            .get();
    }

    template class CrtpBasicTextIterator<TextIterator>;
}// namespace ccl::text

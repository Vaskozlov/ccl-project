#include <ccl/text/text_iterator.hpp>

namespace ccl::text
{
    TextIterator::TextIterator(
        const isl::string_view input,
        const ExceptionHandler &exception_handler,
        const isl::string_view filename)
      : CrtpBasicTextIterator{input}
      , inputInfo{.wholeText = input, .filename = filename}
      , exceptionHandler{std::addressof(exception_handler)}
    {}

    TextIterator::TextIterator(
        const isl::string_view input,
        const ExceptionHandler &exception_handler,
        const Location &iterator_location)
      : CrtpBasicTextIterator{input}
      , inputInfo{.wholeText = input, .filename = {}}
      , location{iterator_location}
      , exceptionHandler{std::addressof(exception_handler)}
    {}

    auto TextIterator::getRealColumn() const -> u32
    {
        if (!isInitialized()) {
            return 0;
        }

        const auto *it = getCarriage();
        const auto *text_begin = inputInfo.wholeText.begin();

        while (it >= text_begin) {
            if (*it == '\n') {
                break;
            }

            --it;
        }

        return isl::as<u32>(std::distance(it, getCarriage()));
    }

    auto TextIterator::nextCharWithEscapingSymbols(const extra_symbols_t &extra_symbols)
        -> isl::Pair<bool, char32_t>
    {
        auto escaping = false;
        char32_t chr = advance();

        if (chr == U'\\') {
            escaping = true;
            chr = doEscapeSymbolizing(*this, extra_symbols);
        }

        setCurrentChar(chr);
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
        const u16 max_times,
        const u16 notation_power,
        const bool need_all_chars) -> char32_t
    {
        return NotationEscapingSequenceToChar(
                   text_iterator, max_times, notation_power, need_all_chars)
            .get();
    }

    auto TextIterator::linesOfFragment(
        const isl::string_view whole_input,
        const isl::string_view fragment) -> isl::string_view
    {
        const auto *front_it = std::next(fragment.begin(), -1);
        const auto *back_it = fragment.end();

        while (front_it >= whole_input.begin()) {
            if (*front_it == U'\n') {
                break;
            }

            front_it = std::next(front_it, -1);
        }

        while (back_it < whole_input.end()) {
            if (*back_it == U'\n') {
                break;
            }

            back_it = std::next(back_it);
        }

        return {std::next(front_it), back_it};
    }

    template class CrtpBasicTextIterator<TextIterator>;
}// namespace ccl::text

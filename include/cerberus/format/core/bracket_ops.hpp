#ifndef CERBERUS_PROJECT_BRACKET_OPS_HPP
#define CERBERUS_PROJECT_BRACKET_OPS_HPP

#include <cerberus/text/basic_text_iterator.hpp>

namespace cerb::fmt::core::bracket
{
    CERBLIB_DECL auto isOpened(const text::BasicTextIterator &text_iterator) -> bool
    {
        return text_iterator.getCurrentChar() == U'{' && text_iterator.futureRawChar(1) != U'{';
    }

    CERBLIB_DECL auto isClosed(const text::BasicTextIterator &text_iterator) -> bool
    {
        return text_iterator.getCurrentChar() == U'}' && text_iterator.futureRawChar(1) != U'}';
    }

    CERBLIB_DECL auto needToSkip(const text::BasicTextIterator &text_iterator) -> bool
    {
        auto chr = text_iterator.getCurrentChar();
        auto future_chr = text_iterator.futureRawChar(1);

        return land(lor(chr == U'{', chr == U'}'), chr == future_chr);
    }
}// namespace cerb::fmt::core::bracket

#endif /* CERBERUS_PROJECT_BRACKET_OPS_HPP */

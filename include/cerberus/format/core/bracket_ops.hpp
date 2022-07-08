#ifndef CERBERUS_PROJECT_BRACKET_OPS_HPP
#define CERBERUS_PROJECT_BRACKET_OPS_HPP

#include <cerberus/text/basic_text_iterator.hpp>

namespace cerb::fmt::core::bracket
{
    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isOpened(const text::BasicTextIterator<CharT> &text_iterator) -> bool
    {
        return text_iterator.getCurrentChar() == '{' && text_iterator.futureRawChar(1) != '{';
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto isClosed(const text::BasicTextIterator<CharT> &text_iterator) -> bool
    {
        return text_iterator.getCurrentChar() == '}' && text_iterator.futureRawChar(1) != '}';
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto needToSkip(const text::BasicTextIterator<CharT> &text_iterator) -> bool
    {
        auto chr = text_iterator.getCurrentChar();

        return (chr == '{' && text_iterator.futureRawChar(1) == '{') ||
               (chr == '}' && text_iterator.futureRawChar(1) == '}');
    }
}// namespace cerb::fmt::core::bracket

#endif /* CERBERUS_PROJECT_BRACKET_OPS_HPP */

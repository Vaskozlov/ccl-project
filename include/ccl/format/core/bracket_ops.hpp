#ifndef CCL_PROJECT_BRACKET_OPS_HPP
#define CCL_PROJECT_BRACKET_OPS_HPP

#include <ccl/text/basic_text_iterator.hpp>

namespace ccl::fmt::core::bracket
{
    CCL_DECL auto isOpened(const text::BasicTextIterator &text_iterator) -> bool
    {
        return text_iterator.getCurrentChar() == U'{' &&
               not text_iterator.isNextCharacterEqual<U'{'>();
    }

    CCL_DECL auto isClosed(const text::BasicTextIterator &text_iterator) -> bool
    {
        return text_iterator.getCurrentChar() == U'}' &&
               not text_iterator.isNextCharacterEqual<U'}'>();
    }

    CCL_DECL auto needToSkip(const text::BasicTextIterator &text_iterator) -> bool
    {
        auto chr = text_iterator.getCurrentChar();

        return lor(chr == U'{', chr == U'}') &&
               chr == static_cast<char32_t>(text_iterator.getNextCarriageValue());
    }
}// namespace ccl::fmt::core::bracket

#endif /* CCL_PROJECT_BRACKET_OPS_HPP */

#ifndef CERBERUS_PROJECT_BASIC_TEXT_ITERATOR_HPP
#define CERBERUS_PROJECT_BASIC_TEXT_ITERATOR_HPP

#include <cerberus/char.hpp>
#include <string_view>

namespace cerb::text
{
    template<CharacterLiteral CharT>
    class BasicTextIterator
    {
    public:
        CERBLIB_DECL auto getCurrentChar() const -> CharT
        {
            if (not initialized) {
                return 0;
            }

            return text[offset];
        }

        CERBLIB_DECL auto getText() const -> const std::basic_string_view<CharT> &
        {
            return text;
        }

        constexpr auto nextRawChar() -> CharT
        {
            if (offset >= text.size()) {
                return 0;
            }

            if (not initialized) {
                initialized = true;
                return text[0];
            }

            return text[++offset];
        }

        constexpr auto nextCleanChar() -> CharT
        {
            while (isLayout(nextRawChar())) {
                // empty loop
            }

            return getCurrentChar();
        }

        CERBLIB_DECL auto futureRawChar(size_t offset_from_current_state) const -> CharT
        {
            auto forked = *this;

            for (size_t i = 0; i != offset_from_current_state; ++i) {
                forked.nextRawChar();
            }

            return forked.getCurrentChar();
        }

        CERBLIB_DECL auto futureCleanChar(size_t offset_from_current_state) const -> CharT
        {
            auto forked = *this;

            for (size_t i = 0; i != offset_from_current_state; ++i) {
                forked.nextCleanChar();
            }

            return forked.getCurrentChar();
        }

        BasicTextIterator() = default;

        constexpr explicit BasicTextIterator(
            std::basic_string_view<CharT> text_, size_t offset_ = 0)
          : text(text_), offset(offset_)
        {}

    private:
        std::basic_string_view<CharT> text{};
        size_t offset{};
        bool initialized{};
    };
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_BASIC_TEXT_ITERATOR_HPP */

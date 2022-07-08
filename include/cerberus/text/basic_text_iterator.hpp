#ifndef CERBERUS_PROJECT_BASIC_TEXT_ITERATOR_HPP
#define CERBERUS_PROJECT_BASIC_TEXT_ITERATOR_HPP

#include <cerberus/char.hpp>
#include <string_view>

namespace cerb::text
{
    template<CharacterLiteral CharT>
    class BasicTextIterator
    {
    private:
        using const_iterator = typename std::basic_string_view<CharT>::const_iterator;

    public:
        CERBLIB_DECL auto getCarriage() const -> const_iterator
        {
            return carriage;
        }

        CERBLIB_DECL auto getRemainingText() const -> std::basic_string_view<CharT>
        {
            return { carriage, end };
        }

        CERBLIB_DECL auto getCurrentChar() const -> CharT
        {
            if (lor(not initialized, carriage == end)) {
                return 0;
            }

            return *carriage;
        }

        virtual constexpr auto nextRawChar() -> CharT
        {
            return basicNextRawChar();
        }

        virtual constexpr auto nextCleanChar() -> CharT
        {
            return basicNextCleanChar();
        }

        constexpr auto basicNextRawChar() -> CharT
        {
            if (carriage == end) {
                return 0;
            }

            if (not initialized) {
                initialized = true;
                return *carriage;
            }

            ++carriage;
            return *carriage;
        }

        constexpr auto basicNextCleanChar() -> CharT
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

        auto operator=(const BasicTextIterator &) -> BasicTextIterator & = default;
        auto operator=(BasicTextIterator &&) noexcept -> BasicTextIterator & = default;

        BasicTextIterator() = default;
        BasicTextIterator(const BasicTextIterator &) = default;
        BasicTextIterator(BasicTextIterator &&) noexcept = default;

        constexpr explicit BasicTextIterator(const std::basic_string_view<CharT> &text_)
          : carriage{ text_.begin() }, end{ text_.end() }
        {}

        virtual ~BasicTextIterator() = default;

    private:
        const_iterator carriage{};
        const_iterator end{};
        bool initialized{};
    };
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_BASIC_TEXT_ITERATOR_HPP */

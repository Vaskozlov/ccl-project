#ifndef CERBERUS_PROJECT_TEXT_ITERATOR_HPP
#define CERBERUS_PROJECT_TEXT_ITERATOR_HPP

#include <cerberus/text/basic_text_iterator.hpp>
#include <cerberus/text/text_iterator_modules/line_tracker.hpp>
#include <cerberus/text/text_iterator_modules/location.hpp>

namespace cerb::text
{
    template<CharacterLiteral CharT>
    class TextIterator : public BasicTextIterator<CharT>
    {
        using Base = BasicTextIterator<CharT>;

    public:
        CERBLIB_DECL auto getLine() const -> size_t
        {
            return location.getLine();
        }

        CERBLIB_DECL auto getColumn() const -> size_t
        {
            return location.getColumn();
        }

        CERBLIB_DECL auto getFilename() const -> const std::string_view &
        {
            return location.getFilename();
        }

        CERBLIB_DECL auto getWorkingLine() const -> const std::basic_string_view<CharT> &
        {
            return line_tracker.get();
        }

        constexpr auto nextRawChar() -> CharT override
        {
            auto chr = Base::basicNextRawChar();
            location.next(chr);
            line_tracker.next();
            return chr;
        }

        constexpr auto nextCleanChar() -> CharT override
        {
            while (isLayout(nextRawChar())) {
                // empty loop
            }

            return Base::getCurrentChar();
        }

        constexpr explicit TextIterator(
            std::basic_string_view<CharT> input, std::string_view filename = "")
          : Base{ input }, location{ filename }
        {}

        ~TextIterator() = default;

    private:
        LineTracker<CharT> line_tracker{ *this };
        Location<char> location{};
    };
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_TEXT_ITERATOR_HPP */

#ifndef CERBERUS_PROJECT_TEXT_ITERATOR_HPP
#define CERBERUS_PROJECT_TEXT_ITERATOR_HPP

#include <cerberus/text/basic_text_iterator.hpp>
#include <cerberus/text/text_iterator_modules/location.hpp>

namespace cerb::text
{
    template<CharacterLiteral CharT>
    class TextIterator : public BasicTextIterator<CharT>
    {
        using Base = BasicTextIterator<CharT>;

    public:
        constexpr auto nextRawChar() -> CharT override
        {
            auto chr = Base::basicNextRawChar();
            location.next(chr);
            return chr;
        }

        constexpr auto nextCleanChar() -> CharT override
        {
            while (isLayout(nextRawChar())) {
                // empty loop
            }

            return Base::getCurrentChar();
        }

    private:
        Location<char> location{};
    };
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_TEXT_ITERATOR_HPP */

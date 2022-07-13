#ifndef CERBERUS_PROJECT_TIM_EXCEPTION_HPP
#define CERBERUS_PROJECT_TIM_EXCEPTION_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/format/format.hpp>
#include <string>

namespace cerb::text
{
    CERBLIB_EXCEPTION(BasicTextIteratorException, CerberusException);

    template<CharacterLiteral CharT>
    class TextIterator;

    template<CharacterLiteral CharT>
    class TextIteratorException : public BasicTextIteratorException
    {
        using StrView = BasicStringView<CharT>;

    public:
        CERBLIB_DECL auto getMessage() const noexcept -> const std::basic_string<CharT> &
        {
            return message;
        }

        [[nodiscard]] auto what() const noexcept -> const char * override
        {
            if constexpr (std::is_same_v<char, CharT>) {
                return message.c_str();
            } else {
                return fmt::format<"{}">(message);
            }
        }

        constexpr TextIteratorException(const TextIterator<CharT> &text_iterator, StrView message_)
          : message(fmt::format<CharT, "Error occurred at: {}, message: {}\n{}\n">(
                text_iterator.getLocation(), message_, text_iterator.getWorkingLine()))
        {
            message.resize(message.size() + text_iterator.getColumn(), ' ');
            message.push_back('^');
        }

    private:
        std::basic_string<CharT> message{};
    };
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_TIM_EXCEPTION_HPP */

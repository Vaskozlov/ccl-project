#ifndef CERBERUS_PROJECT_TIM_EXCEPTION_HPP
#define CERBERUS_PROJECT_TIM_EXCEPTION_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/format/format.hpp>
#include <string>

namespace cerb::text
{
    CERBLIB_EXCEPTION(BasicTextIteratorException, CerberusException);

    template<CharacterLiteral TextT, CharacterLiteral EscapingT = TextT>
    class TextIterator;

    template<CharacterLiteral CharT>
    class TextIteratorException : public BasicTextIteratorException
    {
    public:
        CERBLIB_DECL auto getLine() const -> size_t
        {
            return location.getLine();
        }

        CERBLIB_DECL auto getColumn() const -> size_t
        {
            return location.getColumn();
        }

        CERBLIB_DECL auto getLocation() const -> const Location<char> &
        {
            return location;
        }

        CERBLIB_DECL auto getWorkingLine() const -> const StrView<CharT> &
        {
            return working_line;
        }

        CERBLIB_DECL auto getMessage() const -> StrView<CharT>
        {
            return message;
        }

        CERBLIB_DECL auto getFullMessage() const -> StrView<CharT>
        {
            return full_message;
        }

        CERBLIB_DECL auto getSuggestion() const -> StrView<CharT>
        {
            return suggestion;
        }

        CERBLIB_DECL auto hasSuggestion() const -> bool
        {
            return not suggestion.empty();
        }

        [[nodiscard]] auto what() const noexcept -> const char * override
        {
            if constexpr (std::is_same_v<char, CharT>) {
                return full_message.c_str();
            } else {
                return char_full_message.c_str();
            }
        }

        template<CharacterLiteral MessageT, CharacterLiteral EscapingT>
        constexpr TextIteratorException(
            const TextIterator<CharT, EscapingT> &text_iterator_, StrView<MessageT> message_,
            StrView<MessageT> suggestion_ = {})
          : location(text_iterator_.getLocation()), working_line(text_iterator_.getWorkingLine())
        {
            fmt::dump(message, message_);
            fmt::dump(suggestion, suggestion_);

            createFullMessage();
            createCharFullMessage();
        }

        template<CharacterLiteral MessageT, CharacterLiteral EscapingT>
        constexpr TextIteratorException(
            const TextIterator<CharT, EscapingT> &text_iterator_, const Str<MessageT> &message_,
            const Str<MessageT> &suggestion_ = {})
          : TextIteratorException{ text_iterator_, StrView<MessageT>{ message_ },
                                   StrView<MessageT>{ suggestion_ } }
        {}

        template<CharacterLiteral MessageT, CharacterLiteral EscapingT>
        constexpr TextIteratorException(
            const TextIterator<CharT, EscapingT> &text_iterator_, const MessageT *message_,
            const MessageT *suggestion_ = nullptr)
          : TextIteratorException(
                text_iterator_, StrView<MessageT>{ message_ }, StrView<MessageT>{ suggestion_ })
        {}

    private:
        constexpr auto createFullMessage() -> void
        {
            full_message = fmt::format<CharT, "Error occurred at: {}. Error message: {}\n{}\n">(
                location, message, working_line);

            addArrowToError();
            addSuggestion();
        }

        constexpr auto createCharFullMessage() -> void
        {
            if constexpr (not std::is_same_v<char, CharT>) {
                char_full_message.reserve(full_message.size());
                fmt::dump(char_full_message, full_message);
            }
        }

        constexpr auto addArrowToError() -> void
        {
            auto column_pos = location.getColumn();
            auto new_message_size = full_message.size() + (column_pos > 0 ? column_pos - 1 : 0);
            full_message.resize(new_message_size, ' ');
            full_message.push_back('^');
        }

        constexpr auto addSuggestion() -> void
        {
            if (not suggestion.empty()) {
                full_message.append(fmt::format<CharT, "\nSuggest using: {}">(suggestion));
            }
        }

        Location<char> location{};
        Str<CharT> message{};
        Str<CharT> suggestion{};
        StrView<CharT> working_line{};
        Str<CharT> full_message{};
        Str<char> char_full_message{};
    };
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_TIM_EXCEPTION_HPP */

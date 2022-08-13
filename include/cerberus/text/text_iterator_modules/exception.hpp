#ifndef CERBERUS_PROJECT_TIM_EXCEPTION_HPP
#define CERBERUS_PROJECT_TIM_EXCEPTION_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/format/format.hpp>
#include <string>

namespace cerb::text
{
    CERBLIB_EXCEPTION(BasicTextIteratorException, CerberusException);

    class TextIteratorException : public BasicTextIteratorException
    {
    public:
        [[nodiscard]] auto getLine() const -> size_t
        {
            return location.getLine();
        }

        [[nodiscard]] auto getColumn() const -> size_t
        {
            return location.getColumn();
        }

        [[nodiscard]] auto getLocation() const -> const Location &
        {
            return location;
        }

        [[nodiscard]] auto getWorkingLine() const -> const u8string_view &
        {
            return working_line;
        }

        [[nodiscard]] auto getMessage() const -> u8string_view
        {
            return message;
        }

        [[nodiscard]] auto getSuggestion() const -> u8string_view
        {
            return suggestion;
        }

        [[nodiscard]] auto hasSuggestion() const -> bool
        {
            return not suggestion.empty();
        }

        [[nodiscard]] auto what() const noexcept -> const char * override
        {
            return "unable to return error message (use getMessage or getFullMessage instead)";
        }

        TextIteratorException() = default;

        TextIteratorException(
            const Location &location_, const u8string_view &working_line_, u8string_view message_,
            u8string_view suggestion_ = {})
          : location(location_), message(message_), suggestion(suggestion_),
            working_line(working_line_)
        {}

        TextIteratorException(
            const Location &location_, const u8string_view &working_line_,
            const std::u8string &message_, const std::u8string &suggestion_ = {})
          : TextIteratorException{ location_, working_line_, u8string_view{ message_ },
                                   u8string_view{ suggestion_ } }
        {}

        TextIteratorException(
            const Location &location_, const u8string_view &working_line_, const char8_t *message_,
            const char8_t *suggestion_ = nullptr)
          : TextIteratorException(
                location_, working_line_, u8string_view{ message_ }, u8string_view{ suggestion_ })
        {}

        [[nodiscard]] auto createFullMessage() const -> std::u8string
        {
            auto full_message = fmt::format<u8"Error occurred at: {}, message: {}\n{}\n">(
                location, message, working_line);

            addArrowToError(full_message);
            addSuggestion(full_message);

            return full_message;
        }

        [[nodiscard]] auto createCharFullMessage() const -> std::string
        {
            auto full_message = createFullMessage();
            auto char_full_message = std::string{};

            fmt::dump(char_full_message, full_message);

            return char_full_message;
        }

    private:
        auto addArrowToError(std::u8string &full_message) const -> void
        {
            auto column_pos = location.getColumn();
            auto new_message_size = full_message.size() + (column_pos > 0 ? column_pos - 1 : 0);
            full_message.resize(new_message_size, ' ');
            full_message.push_back('^');
        }

        auto addSuggestion(std::u8string &full_message) const -> void
        {
            if (not suggestion.empty()) {
                full_message.append(fmt::format<u8"\nSuggestion: {}">(suggestion));
            }
        }

        Location location{};
        std::u8string message{};
        std::u8string suggestion{};
        u8string_view working_line{};
    };
}// namespace cerb::text

#endif /* CERBERUS_PROJECT_TIM_EXCEPTION_HPP */

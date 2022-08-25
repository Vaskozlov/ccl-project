#ifndef CCL_PROJECT_TIM_EXCEPTION_HPP
#define CCL_PROJECT_TIM_EXCEPTION_HPP

#include <ccl/format/format.hpp>
#include <string>

namespace ccl::text
{
    CCL_EXCEPTION(BasicTextIteratorException, CclException);

    class TextIteratorException : public BasicTextIteratorException
    {
    public:
        [[nodiscard]] auto getLine() const noexcept -> size_t
        {
            return location.getLine();
        }

        [[nodiscard]] auto getColumn() const noexcept -> size_t
        {
            return location.getColumn();
        }

        [[nodiscard]] auto getLocation() const noexcept -> const Location &
        {
            return location;
        }

        [[nodiscard]] auto getWorkingLine() const noexcept -> const u8string_view &
        {
            return working_line;
        }

        [[nodiscard]] auto getMessage() const noexcept -> u8string_view
        {
            return message;
        }

        [[nodiscard]] auto getSuggestion() const noexcept -> u8string_view
        {
            return suggestion;
        }

        [[nodiscard]] auto hasSuggestion() const noexcept -> bool
        {
            return not suggestion.empty();
        }

        [[nodiscard]] auto what() const noexcept -> const char * override;

        TextIteratorException() = default;

        TextIteratorException(
            const Location &location_, const u8string_view &working_line_,
            const u8string_view &message_, const u8string_view &suggestion_ = {})
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

        [[nodiscard]] auto createFullMessage() const -> std::u8string;

    private:
        auto addSuggestion(std::u8string &full_message) const -> void;
        auto addArrowToError(std::u8string &full_message) const -> void;

        Location location{};
        std::u8string message{};
        std::u8string suggestion{};
        u8string_view working_line{};
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_TIM_EXCEPTION_HPP */

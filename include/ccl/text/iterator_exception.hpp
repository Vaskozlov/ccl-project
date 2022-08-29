#ifndef CCL_PROJECT_TIM_EXCEPTION_HPP
#define CCL_PROJECT_TIM_EXCEPTION_HPP

#include <ccl/text/location.hpp>
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

        [[nodiscard]] auto getFilename() const noexcept -> const string_view &
        {
            return location.getFilename();
        }

        [[nodiscard]] auto getLocation() const noexcept -> const Location &
        {
            return location;
        }

        [[nodiscard]] auto getWorkingLine() const noexcept -> const string_view &
        {
            return working_line;
        }

        [[nodiscard]] auto getMessage() const noexcept -> string_view
        {
            return message;
        }

        [[nodiscard]] auto getSuggestion() const noexcept -> string_view
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
            const Location &location_, const string_view &working_line_,
            const string_view &message_, const string_view &suggestion_ = {})
          : location(location_), message(message_), suggestion(suggestion_),
            working_line(working_line_)
        {}

        CCL_PERFECT_FORWARDING_2(T1, std::string, T2, std::string)
        TextIteratorException(
            const Location &location_, const string_view &working_line_, T1 &&message_,
            T2 &&suggestion_ = {})
          : location(location_), message(std::forward<T1>(message_)),
            suggestion(std::forward<T2>(suggestion_)), working_line(working_line_)
        {}

        [[nodiscard]] auto createFullMessage() const -> std::string;

    private:
        auto addSuggestion(std::string &full_message) const -> void;
        auto addArrowToError(std::string &full_message) const -> void;

        Location location{};
        std::string message{};
        std::string suggestion{};
        string_view working_line{};
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_TIM_EXCEPTION_HPP */

#ifndef CCL_PROJECT_TIM_EXCEPTION_HPP
#define CCL_PROJECT_TIM_EXCEPTION_HPP

#include <ccl/text/location.hpp>
#include <string>

namespace ccl
{
    CCL_EXCEPTION(BasicTextIteratorException, CclException);

    CCL_ENUM(// NOLINTNEXTLINE
        ExceptionCriticality, u32, NONE, SUGGESTION, WARNING, UNCRITICAL, CRITICAL, PANIC);

    // NOLINTNEXTLINE
    CCL_ENUM(AnalysationStage, u32, NONE, LEXICAL_ANALYSIS, PARSING);

    auto ExceptionCriticalityDescription(ExceptionCriticality criticality) noexcept
        -> std::string_view;
}// namespace ccl

namespace ccl::text
{
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

        [[nodiscard]] auto getCriticality() const noexcept -> ExceptionCriticality
        {
            return criticality;
        }

        [[nodiscard]] auto getCriticalityDescription() const noexcept -> string_view
        {
            return ExceptionCriticalityDescription(criticality);
        }

        [[nodiscard]] auto getStage() const noexcept -> AnalysationStage
        {
            return stage;
        }

        [[nodiscard]] auto hasSuggestion() const noexcept -> bool
        {
            return not suggestion.empty();
        }

        [[nodiscard]] auto what() const noexcept -> const char * override;

        TextIteratorException() = default;

        TextIteratorException(
            ExceptionCriticality criticality_, const Location &location_,
            const string_view &working_line_, const string_view &message_,
            const string_view &suggestion_ = {})
          : location(location_), message(message_), suggestion(suggestion_),
            working_line(working_line_), criticality(criticality_)
        {}

        CCL_PERFECT_FORWARDING_2(T1, std::string, T2, std::string)
        TextIteratorException(
            ExceptionCriticality criticality_, const Location &location_,
            const string_view &working_line_, T1 &&message_, T2 &&suggestion_ = {})
          : location(location_), message(std::forward<T1>(message_)),
            suggestion(std::forward<T2>(suggestion_)), working_line(working_line_),
            criticality(criticality_)
        {}

        [[nodiscard]] auto createFullMessage() const -> std::string;

    private:
        auto addSuggestion(std::string &full_message) const -> void;
        auto addArrowToError(std::string &full_message) const -> void;

        Location location{};
        std::string message{};
        std::string suggestion{};
        string_view working_line{};
        ExceptionCriticality criticality{};
        AnalysationStage stage{};
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_TIM_EXCEPTION_HPP */

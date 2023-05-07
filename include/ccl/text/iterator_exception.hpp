#ifndef CCL_PROJECT_ITERATOR_EXCEPTION_HPP
#define CCL_PROJECT_ITERATOR_EXCEPTION_HPP

#include <ccl/text/location.hpp>

namespace ccl
{
    CCL_EXCEPTION(BasicTextIteratorException, std::exception);

    enum struct ExceptionCriticality : u32
    {
        NONE,
        SUGGESTION,
        WARNING,
        UNCRITICAL,
        CRITICAL,
        PANIC
    };

    enum struct AnalysisStage : u32
    {
        NONE,
        LEXICAL_ANALYSIS,
        PARSING
    };

    auto exceptionCriticalityDescription(ExceptionCriticality criticality) noexcept
        -> std::string_view;
}// namespace ccl

namespace ccl::text
{
    class TextIteratorException : public BasicTextIteratorException
    {
    private:
        Location location{};
        std::string message{};
        std::string suggestion{};
        string_view workingLine{};
        size_t length{1};
        ExceptionCriticality criticality{};
        AnalysisStage stage{};

    public:
        TextIteratorException() = default;

        [[nodiscard]] TextIteratorException(
            ExceptionCriticality exception_criticality, AnalysisStage analysis_stage,
            const Location &exception_location, size_t exception_length, string_view working_line,
            std::string exception_message, std::string exception_suggestion = std::string{});

        [[nodiscard]] auto getLine() const noexcept -> size_t
        {
            return location.getLine();
        }

        [[nodiscard]] auto getColumn() const noexcept -> size_t
        {
            return location.getColumn();
        }

        [[nodiscard]] auto getLength() const noexcept -> size_t
        {
            return length;
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
            return workingLine;
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
            return exceptionCriticalityDescription(criticality);
        }

        [[nodiscard]] auto getStage() const noexcept -> AnalysisStage
        {
            return stage;
        }

        [[nodiscard]] auto hasSuggestion() const noexcept -> bool
        {
            return !suggestion.empty();
        }

        [[nodiscard]] auto what() const noexcept -> const char * override;

        [[nodiscard]] auto createFullMessage() const -> std::string;

    private:
        auto addSuggestion(std::string &full_message) const -> void;
        auto addArrowToError(std::string &full_message) const -> void;
    };
}// namespace ccl::text

#endif /* CCL_PROJECT_ITERATOR_EXCEPTION_HPP */

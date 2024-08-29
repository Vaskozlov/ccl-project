#ifndef CCL_PROJECT_ITERATOR_EXCEPTION_HPP
#define CCL_PROJECT_ITERATOR_EXCEPTION_HPP

#include <ccl/text/location.hpp>

namespace ccl
{
    ISL_EXCEPTION(BasicTextIteratorException, std::exception, exception);

    enum struct ExceptionCriticality : u8
    {
        NONE,
        SUGGESTION,
        WARNING,
        UNCRITICAL,
        CRITICAL,
        PANIC
    };

    enum struct AnalysisStage : u8
    {
        NONE,
        LEXICAL_ANALYSIS,
        PARSING
    };
}// namespace ccl

namespace ccl::text
{
    class TextIteratorException : public BasicTextIteratorException
    {
    private:
        std::string message;
        std::string suggestion;
        std::string filename;
        std::string workingLine;
        Location location;
        std::size_t length{1};
        ExceptionCriticality criticality{ExceptionCriticality::NONE};
        AnalysisStage stage{AnalysisStage::NONE};

    public:
        TextIteratorException() = default;

        [[nodiscard]] TextIteratorException(
            ExceptionCriticality exception_criticality, AnalysisStage analysis_stage,
            Location exception_location, std::string name_of_file, std::size_t exception_length,
            std::string working_line, std::string exception_message,
            std::string exception_suggestion = std::string{});

        [[nodiscard]] auto getLine() const noexcept -> std::size_t
        {
            return location.line;
        }

        [[nodiscard]] auto getColumn() const noexcept -> std::size_t
        {
            return location.column;
        }

        [[nodiscard]] auto getLength() const noexcept -> std::size_t
        {
            return length;
        }

        [[nodiscard]] auto getFilename() const noexcept -> isl::string_view
        {
            return filename;
        }

        [[nodiscard]] auto getLocation() const noexcept CCL_LIFETIMEBOUND -> const Location &
        {
            return location;
        }

        [[nodiscard]] auto getWorkingLine() const noexcept CCL_LIFETIMEBOUND -> const std::string &
        {
            return workingLine;
        }

        [[nodiscard]] auto getMessage() const noexcept CCL_LIFETIMEBOUND -> const std::string &
        {
            return message;
        }

        [[nodiscard]] auto getSuggestion() const noexcept CCL_LIFETIMEBOUND -> const std::string &
        {
            return suggestion;
        }

        [[nodiscard]] auto getCriticality() const noexcept -> ExceptionCriticality
        {
            return criticality;
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

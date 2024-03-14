module;
#include <ccl/defines.hpp>
export module ccl.text.core:iterator_exception;

export import ccl.core;
export import :location;

export namespace ccl::text
{
    ISL_EXCEPTION(BasicTextIteratorException, std::exception, exception);

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
}// namespace ccl::text

export namespace ccl::text
{
    class TextIteratorException : public BasicTextIteratorException
    {
    private:
        Location location;
        std::string message;
        std::string suggestion;
        isl::string_view workingLine;
        std::size_t length{1};
        ExceptionCriticality criticality{ExceptionCriticality::NONE};
        AnalysisStage stage{AnalysisStage::NONE};

    public:
        TextIteratorException() = default;

        [[nodiscard]] TextIteratorException(
            ExceptionCriticality exception_criticality, AnalysisStage analysis_stage,
            const Location &exception_location, std::size_t exception_length,
            isl::string_view working_line, std::string exception_message,
            std::string exception_suggestion = std::string{})
        : location{exception_location}
        , message{std::move(exception_message)}
        , suggestion{std::move(exception_suggestion)}
        , workingLine{working_line}
        , length{exception_length}
        , criticality{exception_criticality}
        , stage{analysis_stage}
        {}

        [[nodiscard]] auto getLine() const noexcept -> std::size_t
        {
            return location.getLine();
        }

        [[nodiscard]] auto getColumn() const noexcept -> std::size_t
        {
            return location.getColumn();
        }

        [[nodiscard]] auto getLength() const noexcept -> std::size_t
        {
            return length;
        }

        [[nodiscard]] auto getFilename() const noexcept -> isl::string_view
        {
            return location.getFilename();
        }

        [[nodiscard]] auto getLocation() const noexcept CCL_LIFETIMEBOUND -> const Location &
        {
            return location;
        }

        [[nodiscard]] auto
            getWorkingLine() const noexcept CCL_LIFETIMEBOUND -> const isl::string_view &
        {
            return workingLine;
        }

        [[nodiscard]] auto getMessage() const noexcept -> isl::string_view
        {
            return message;
        }

        [[nodiscard]] auto getSuggestion() const noexcept -> isl::string_view
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

        [[nodiscard]] auto what() const noexcept -> const char * override
        {
            return message.c_str();
        }

        [[nodiscard]] auto createFullMessage() const -> std::string
        {
            auto full_message = std::format(
                "Error occurred at: {}, line: {}, column: {}, message: {}\n{}\n",
                location.getFilename(), location.getLine(), location.getColumn(), message, workingLine);

            addArrowToError(full_message);
            addSuggestion(full_message);

            return full_message;
        }

    private:
        auto addSuggestion(std::string &full_message) const -> void
        {
            if (!suggestion.empty()) {
                full_message.append(std::format("\nSuggestion: {}", suggestion));
            }
        }

        auto addArrowToError(std::string &full_message) const -> void
        {
            auto column_pos = location.getColumn();
            auto new_message_size = full_message.size() + (column_pos > 0 ? column_pos - 1 : 0);

            full_message.resize(new_message_size, ' ');
            full_message.push_back('^');
        }
    };
}// namespace ccl::text

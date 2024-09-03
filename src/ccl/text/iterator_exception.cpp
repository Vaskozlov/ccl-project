#include <ccl/text/iterator_exception.hpp>

namespace ccl::text
{
    TextIteratorException::TextIteratorException(
        const ExceptionCriticality exception_criticality, const AnalysisStage analysis_stage,
        const Location exception_location, std::string name_of_file,
        const std::size_t exception_length, std::string working_line, std::string exception_message,
        std::string exception_suggestion)
      : message{std::move(exception_message)}
      , suggestion{std::move(exception_suggestion)}
      , filename{std::move(name_of_file)}
      , workingLine{std::move(working_line)}
      , location{exception_location}
      , length{exception_length}
      , criticality{exception_criticality}
      , stage{analysis_stage}
    {}

    auto TextIteratorException::what() const noexcept -> const char *
    {
        return message.c_str();
    }

    auto TextIteratorException::createFullMessage() const -> std::string
    {
        auto full_message = fmt::format(
            "Error occurred at: {}, {}, message: {}\n{}\n", filename, location, message,
            workingLine);

        addArrowToError(full_message);
        addSuggestion(full_message);

        return full_message;
    }

    auto TextIteratorException::addArrowToError(std::string &full_message) const -> void
    {
        const auto column_pos = location.column;
        const auto new_message_size = full_message.size() + (column_pos > 0 ? column_pos - 1 : 0);

        full_message.resize(new_message_size, ' ');
        full_message.push_back('^');
    }

    auto TextIteratorException::addSuggestion(std::string &full_message) const -> void
    {
        if (!suggestion.empty()) {
            full_message.append(fmt::format("\nSuggestion: {}", suggestion));
        }
    }
}// namespace ccl::text

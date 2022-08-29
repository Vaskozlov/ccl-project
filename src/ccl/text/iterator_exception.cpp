#include <ccl/text/iterator_exception.hpp>

namespace ccl::text
{
    using namespace ccl::string_view_literals;

    auto TextIteratorException::what() const noexcept -> const char *
    {
        return message.c_str();
    }

    auto TextIteratorException::createFullMessage() const -> std::string
    {
        auto full_message = ::fmt::format(
            "Error occurred at: {}, message: {}\n{}\n", location, message, working_line);

        addArrowToError(full_message);
        addSuggestion(full_message);

        return full_message;
    }

    auto TextIteratorException::addArrowToError(std::string &full_message) const -> void
    {
        auto column_pos = location.getColumn();
        auto new_message_size = full_message.size() + (column_pos > 0 ? column_pos - 1 : 0);
        full_message.resize(new_message_size, ' ');
        full_message.push_back('^');
    }

    auto TextIteratorException::addSuggestion(std::string &full_message) const -> void
    {
        if (not suggestion.empty()) {
            full_message.append(::fmt::format("\nSuggestion: {}", suggestion));
        }
    }
}// namespace ccl::text

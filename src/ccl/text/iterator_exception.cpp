#include <ccl/text/iterator_exception.hpp>

namespace ccl::text
{
    using namespace ccl::string_view_literals;

    auto TextIteratorException::what() const noexcept -> const char *
    {
        return reinterpret_cast<const char *>(message.data());// NOLINT at least ascii
                                                              // characters will be displayed
    }

    auto TextIteratorException::createFullMessage() const -> std::u8string
    {
        auto full_message = fmt::format<u8"Error occurred at: {}, message: {}\n{}\n">(
            location, message, working_line);

        addArrowToError(full_message);
        addSuggestion(full_message);

        return full_message;
    }

    auto TextIteratorException::addArrowToError(std::u8string &full_message) const -> void
    {
        auto column_pos = location.getColumn();
        auto new_message_size = full_message.size() + (column_pos > 0 ? column_pos - 1 : 0);
        full_message.resize(new_message_size, u8' ');
        full_message.push_back(u8'^');
    }

    auto TextIteratorException::addSuggestion(std::u8string &full_message) const -> void
    {
        if (not suggestion.empty()) {
            full_message.append(fmt::format<u8"\nSuggestion: {}">(suggestion));
        }
    }
}// namespace ccl::text

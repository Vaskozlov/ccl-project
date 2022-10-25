#ifndef CCL_PROJECT_CMD_HANDLER_HPP
#define CCL_PROJECT_CMD_HANDLER_HPP

#include <ccl/handler/exception_handler.hpp>
#include <ccl/text/text_iterator.hpp>
#include <iostream>

namespace ccl::handler
{
    struct Cmd final : ExceptionHandler
    {
    private:
        class CmdFormatter;

    public:
        using ExceptionHandler::ExceptionT;

        Cmd() noexcept = default;

        static auto instance() -> Cmd &;

    private:
        auto onHandle(const ExceptionT *instance) -> void final;

        template<fmt::color Color, typename HandleType>
        auto formatAndPrint(const ExceptionT *value, HandleType &&handle_type) -> void;
    };

    class Cmd::CmdFormatter
    {
        std::string formatting_buffer{};
        string_view working_line{};
        string_view filename{};
        string_view message{};
        string_view suggestion{};
        size_t length{};
        size_t line{};
        size_t column{};

    public:
        explicit CmdFormatter(const ExceptionT *exception) noexcept;

        template<fmt::color Color, typename HandleType>
        auto format(HandleType &&handle_type) -> std::string
        {
            formatFilename();
            formatErrorCategory<Color>(std::forward<HandleType>(handle_type));
            formatMessage();
            formatLineNumber();

            auto tabs_count = formatWorkingLine();
            auto arrow_position = formatArrow(tabs_count);

            formatSuggestion(arrow_position);

            return formatting_buffer;
        }

    private:
        auto formatFilename() -> void;

        template<fmt::color Color, typename HandleType>
        auto formatErrorCategory(HandleType &&handle_type) -> void
        {
            format_to(std::back_inserter(formatting_buffer), fg(Color), handle_type);
        }

        auto formatMessage() -> void;

        auto formatLineNumber() -> void;

        auto formatWorkingLine() -> size_t;

        auto formatArrow(size_t tabs_number) -> size_t;

        auto formatSuggestion(size_t arrow_position) -> void;
    };
}// namespace ccl::handler

#endif /* CCL_PROJECT_CMD_HANDLER_HPP */

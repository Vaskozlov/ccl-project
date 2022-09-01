#ifndef CCL_PROJECT_CMD_HANDLER_HPP
#define CCL_PROJECT_CMD_HANDLER_HPP

#include <ccl/handler/exception_handler.hpp>
#include <ccl/text/text_iterator.hpp>
#include <iostream>

namespace ccl::handler
{
    struct Cmd : ExceptionHandler
    {
        using ExceptionHandler::ExceptionT;

        Cmd() noexcept = default;

        static auto instance() -> Cmd &;

    private:
        struct CmdFormatter
        {
            explicit CmdFormatter(const ExceptionT *exception) noexcept
              : working_line(exception->getWorkingLine()), filename(exception->getFilename()),
                message(exception->getMessage()), suggestion(exception->getSuggestion()),
                line(exception->getLine()), column(exception->getColumn())
            {}

            template<fmt::color Color, typename HandleType>
            auto format(HandleType &&handle_type) -> std::string;

        private:
            auto formatFilename() -> void;

            template<fmt::color Color, typename HandleType>
            auto formatErrorCategory(HandleType &&handle_type) -> void;

            auto formatMessage() -> void;

            auto formatLineNumber() -> void;

            auto formatWorkingLine() -> size_t;

            auto formatArrow(size_t tabs_number) -> size_t;

            auto formatSuggestion(size_t arrow_position) -> void;

            std::string formatting_buffer{};
            string_view working_line{};
            string_view filename{};
            string_view message{};
            string_view suggestion{};
            size_t line{};
            size_t column{};
        };

        auto onHandle(const ExceptionT *instance) -> void override;

        template<fmt::color Color, typename HandleType>
        auto formatAndPrint(const ExceptionT *value, HandleType &&handle_type) -> void;
    };
}// namespace ccl::handler

#endif /* CCL_PROJECT_CMD_HANDLER_HPP */

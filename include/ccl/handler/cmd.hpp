#ifndef CCL_PROJECT_CMD_HPP
#define CCL_PROJECT_CMD_HPP

#include <ccl/handler/exception_handler.hpp>
#include <ccl/text/text_iterator.hpp>
#include <iostream>

namespace ccl::handler
{
    class Cmd final : public ExceptionHandler
    {
    private:
        static Cmd defaultCmdHandler;

        class CmdFormatter;

    public:
        using ExceptionHandler::ExceptionT;

        Cmd() noexcept = default;

        [[nodiscard]] static auto instance() -> Cmd &;

    private:
        auto onHandle(const ExceptionT *error) -> void override;

        template<fmt::color Color, typename HandleType>
        auto formatAndPrint(const ExceptionT *value, HandleType &&handle_type) -> void;
    };

    class Cmd::CmdFormatter
    {
        friend class Cmd;

        std::string formattingBuffer{};
        string_view workingLine{};
        string_view filename{};
        string_view message{};
        string_view suggestion{};
        size_t length{};
        size_t line{};
        size_t column{};

    public:
        explicit CmdFormatter(const ExceptionT *exception) noexcept;

    private:
        template<fmt::color Color, typename HandleType>
        [[nodiscard]] auto format(HandleType &&handle_type) -> std::string;

        auto formatFilename() -> void;

        template<fmt::color Color, typename HandleType>
        auto formatErrorCategory(HandleType &&handle_type) -> void;

        auto formatMessage() -> void;

        auto formatLineNumber() -> void;

        auto formatWorkingLine() -> size_t;

        [[nodiscard]] auto formatArrow(size_t tabs_number) -> size_t;

        auto formatSuggestion(size_t arrow_position) -> void;
    };
}// namespace ccl::handler

#endif /* CCL_PROJECT_CMD_HPP */

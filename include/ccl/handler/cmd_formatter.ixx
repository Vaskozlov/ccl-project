module;
#include <fmt/color.h>
#include <fmt/std.h>
export module ccl.handler:cmd_formatter;

export import :excaption_handler;

export namespace ccl::handler
{
    class CmdFormatter
    {
    private:
        std::string formattingBuffer{};
        isl::string_view workingLine{};
        isl::string_view filename{};
        isl::string_view message{};
        isl::string_view suggestion{};
        std::size_t length{};
        std::size_t line{};
        std::size_t column{};

    public:
        using ExceptionT = ExceptionHandler::ExceptionT;

        explicit CmdFormatter(const ExceptionT *exception) noexcept;

        template<fmt::color Color, typename HandleType>
        [[nodiscard]] auto format(HandleType &&handle_type) -> std::string
        {
            formatFilename();
            formatErrorCategory<Color>(std::forward<HandleType>(handle_type));
            formatMessage();
            formatLineNumber();

            auto tabs_count = formatWorkingLine();
            auto arrow_position = formatArrow(tabs_count);

            formatSuggestion(arrow_position);

            return formattingBuffer;
        }

    private:
        auto formatFilename() -> void;

        template<fmt::color Color, typename HandleType>
        auto formatErrorCategory(HandleType &&handle_type) -> void
        {
            fmt::format_to(
                std::back_inserter(formattingBuffer), fg(Color),
                std::forward<HandleType>(handle_type));
        }

        auto formatMessage() -> void;

        auto formatLineNumber() -> void;

        auto formatWorkingLine() -> std::size_t;

        [[nodiscard]] auto formatArrow(std::size_t tabs_number) -> std::size_t;

        auto formatSuggestion(std::size_t arrow_position) -> void;
    };
}// namespace ccl::handler

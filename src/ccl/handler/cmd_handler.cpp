#include <ccl/handler/cmd_handler.hpp>

namespace ccl::handler
{
    static auto replaceTabWithSpaces(const string_view &string) -> std::pair<size_t, std::string>
    {
        auto result = std::string{};
        size_t replaced_tabs = 0;

        for (auto &&chr : string) {
            if (chr == '\t') {
                ++replaced_tabs;
                result += "    ";
            } else {
                result += chr;
            }
        }

        return { replaced_tabs, result };
    }

    template<fmt::color Color, typename HandleType>
    auto Cmd::CmdFormatter::format(HandleType &&handle_type) -> std::string
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

    auto Cmd::CmdFormatter::formatFilename() -> void
    {
        fmt::format_to(
            std::back_inserter(formatting_buffer),
            fmt::emphasis::underline | fmt::fg(fmt::color::cornflower_blue), "{}:{}:{}", filename,
            line, column);

        fmt::format_to(
            std::back_inserter(formatting_buffer), fmt::fg(fmt::color::cornflower_blue), ":");
    }

    template<fmt::color Color, typename HandleType>
    auto Cmd::CmdFormatter::formatErrorCategory(HandleType &&handle_type) -> void
    {
        fmt::format_to(std::back_inserter(formatting_buffer), fmt::fg(Color), handle_type);
    }

    auto Cmd::CmdFormatter::formatMessage() -> void
    {
        fmt::format_to(std::back_inserter(formatting_buffer), "{}\n", message);
    }

    auto Cmd::CmdFormatter::formatLineNumber() -> void
    {
        fmt::format_to(std::back_inserter(formatting_buffer), "{:>4} {:<6}", line, '|');
    }

    auto Cmd::CmdFormatter::formatWorkingLine() -> size_t
    {
        auto [tabs_number, line_to_print] = replaceTabWithSpaces(working_line);

        fmt::format_to(
            std::back_inserter(formatting_buffer), fmt::emphasis::italic, "{}\n", line_to_print);

        return tabs_number;
    }

    auto Cmd::CmdFormatter::formatArrow(size_t tabs_number) -> size_t
    {
        constexpr auto total_line_number_width = 6;
        auto arrow_position = column + tabs_number * (4 - 1) + total_line_number_width - 1;

        fmt::format_to(std::back_inserter(formatting_buffer), "{:>6}", '|');

        fmt::format_to(
            std::back_inserter(formatting_buffer), fmt::fg(fmt::color::light_green), "{:>{}}\n",
            "^", arrow_position);

        return arrow_position;
    }

    auto Cmd::CmdFormatter::formatSuggestion(size_t arrow_position) -> void
    {
        if (!suggestion.empty()) {
            fmt::format_to(std::back_inserter(formatting_buffer), "{:>6}", '|');
            fmt::format_to(
                std::back_inserter(formatting_buffer), "{1:>{0}} {2}\n",
                std::max<size_t>(1, arrow_position), "Suggest to", suggestion);
        }
    }

    auto Cmd::instance() -> Cmd &
    {
        static auto handler = Cmd{};
        return handler;
    }

    auto Cmd::onHandle(const ExceptionT *instance) -> void
    {
        switch (instance->getCriticality()) {
        case ExceptionCriticality::SUGGESTION:
            formatAndPrint<fmt::color::white>(instance, " suggestion: ");
            break;

        case ExceptionCriticality::WARNING:
            formatAndPrint<fmt::color::medium_violet_red>(instance, " warning: ");
            break;

        default:
            formatAndPrint<fmt::color::red>(instance, " error: ");
            break;
        }
    }

    template<fmt::color Color, typename HandleType>
    auto Cmd::formatAndPrint(const ExceptionT *value, HandleType &&handle_type) -> void
    {
        auto formatter = CmdFormatter{ value };
        fmt::print("{}\n", formatter.format<Color>(std::forward<HandleType>(handle_type)));
        std::cout.flush();
    }
}// namespace ccl::handler

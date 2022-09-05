#include <ccl/handler/cmd_handler.hpp>

using namespace fmt;

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

    auto Cmd::CmdFormatter::formatFilename() -> void
    {
        format_to(
            std::back_inserter(formatting_buffer), emphasis::underline | fg(color::cornflower_blue),
            "{}:{}:{}", filename, line, column);

        format_to(std::back_inserter(formatting_buffer), fg(color::cornflower_blue), ":");
    }

    auto Cmd::CmdFormatter::formatMessage() -> void
    {
        format_to(std::back_inserter(formatting_buffer), "{}\n", message);
    }

    auto Cmd::CmdFormatter::formatLineNumber() -> void
    {
        format_to(std::back_inserter(formatting_buffer), "{:>4} {:<6}", line, '|');
    }

    auto Cmd::CmdFormatter::formatWorkingLine() -> size_t
    {
        auto [tabs_number, line_to_print] = replaceTabWithSpaces(working_line);

        format_to(std::back_inserter(formatting_buffer), emphasis::italic, "{}\n", line_to_print);
        return tabs_number;
    }

    auto Cmd::CmdFormatter::formatArrow(size_t tabs_number) -> size_t
    {
        constexpr auto total_line_number_width = 6;
        auto arrow_position = column + tabs_number * (4 - 1) + total_line_number_width - 1;

        format_to(std::back_inserter(formatting_buffer), "{:>6}", '|');

        format_to(
            std::back_inserter(formatting_buffer), fg(color::light_green), "{:>{}}\n", "^",
            arrow_position);

        return arrow_position;
    }

    auto Cmd::CmdFormatter::formatSuggestion(size_t arrow_position) -> void
    {
        if (!suggestion.empty()) {
            format_to(std::back_inserter(formatting_buffer), "{:>6}", '|');
            format_to(
                std::back_inserter(formatting_buffer), "{1:>{0}} {2}\n",
                std::max<size_t>(1, arrow_position), "Suggest to", suggestion);
        }
    }
}// namespace ccl::handler

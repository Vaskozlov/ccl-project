#include <ccl/handler/cmd.hpp>

using namespace fmt;

namespace ccl::handler
{
    using namespace std::string_view_literals;

    static auto replaceTabWithSpaces(string_view string) -> Pair<size_t, std::string>
    {
        auto result = std::string{};
        auto replaced_tabs = as<size_t>(0);

        CCL_UNROLL_N(4)
        for (auto chr : string) {
            if ('\t' == chr) {
                ++replaced_tabs;
                result.append("    "sv);
            } else {
                result.push_back(chr);
            }
        }

        return {replaced_tabs, result};
    }

    auto Cmd::CmdFormatter::formatFilename() -> void
    {
        fmt::format_to(
            std::back_inserter(formattingBuffer), emphasis::underline | fg(color::cornflower_blue),
            "{}:{}:{}", filename, line, column);

        fmt::format_to(std::back_inserter(formattingBuffer), fg(color::cornflower_blue), ":");
    }

    auto Cmd::CmdFormatter::formatMessage() -> void
    {
        fmt::format_to(std::back_inserter(formattingBuffer), "{}\n", as<std::string_view>(message));
    }

    auto Cmd::CmdFormatter::formatLineNumber() -> void
    {
        fmt::format_to(std::back_inserter(formattingBuffer), "{:>4} {:<6}", line, '|');
    }

    auto Cmd::CmdFormatter::formatWorkingLine() -> size_t
    {
        auto [tabs_number, line_to_print] = replaceTabWithSpaces(workingLine);

        fmt::format_to(std::back_inserter(formattingBuffer), emphasis::italic, "{}\n", line_to_print);
        return tabs_number;
    }

    auto Cmd::CmdFormatter::formatArrow(size_t tabs_number) -> size_t
    {
        constexpr auto total_line_number_width = 6U;
        auto arrow_position =
            (column - 1) + length + tabs_number * (4 - 1) + total_line_number_width - 1;

        fmt::format_to(std::back_inserter(formattingBuffer), "{:>6}", '|');

        fmt::format_to(
            std::back_inserter(formattingBuffer), fg(color::light_green), "{:>{}}\n",
            std::string(length, '^'), arrow_position);

        return arrow_position;
    }

    auto Cmd::CmdFormatter::formatSuggestion(size_t arrow_position) -> void
    {
        if (!suggestion.empty()) {
            fmt::format_to(std::back_inserter(formattingBuffer), "{:>6}", '|');
            fmt::format_to(
                std::back_inserter(formattingBuffer), "{1:>{0}} {2}\n",
                std::max<size_t>(1, arrow_position),
                "Suggestion: ", as<std::string_view>(suggestion));
        }
    }
}// namespace ccl::handler

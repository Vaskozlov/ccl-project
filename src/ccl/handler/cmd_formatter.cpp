#include <ccl/handler/cmd.hpp>

namespace ccl::handler
{
    static auto replaceTabWithSpaces(isl::string_view string) -> isl::Pair<std::size_t, std::string>
    {
        using namespace std::string_view_literals;

        auto result = std::string{};
        auto replaced_tabs = isl::as<std::size_t>(0);

        ISL_UNROLL_N(4)
        for (const auto chr : string) {
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
            std::back_inserter(formattingBuffer),
            fmt::emphasis::underline | fg(fmt::color::cornflower_blue), "{}:{}:{}", filename, line,
            column);

        fmt::format_to(std::back_inserter(formattingBuffer), fg(fmt::color::cornflower_blue), ":");
    }

    auto Cmd::CmdFormatter::formatMessage() -> void
    {
        fmt::format_to(std::back_inserter(formattingBuffer), "{}\n", message);
    }

    auto Cmd::CmdFormatter::formatLineNumber() -> void
    {
        fmt::format_to(std::back_inserter(formattingBuffer), "{:>4} {:<6}", line, '|');
    }

    auto Cmd::CmdFormatter::formatWorkingLine() -> std::size_t
    {
        auto [tabs_number, line_to_print] = replaceTabWithSpaces(workingLine);

        fmt::format_to(
            std::back_inserter(formattingBuffer), fmt::emphasis::italic, "{}\n", line_to_print);
        return tabs_number;
    }

    auto Cmd::CmdFormatter::formatArrow(const std::size_t tabs_number) -> std::size_t
    {
        constexpr auto total_line_number_width = 6U;
        auto arrow_position =
            (column - 1) + length + (tabs_number * (4 - 1)) + (total_line_number_width - 1);

        fmt::format_to(std::back_inserter(formattingBuffer), "{:>6}", '|');

        fmt::format_to(
            std::back_inserter(formattingBuffer), fmt::fg(fmt::color::light_green), "{:>{}}\n",
            std::string(length, '^'), arrow_position);

        return arrow_position;
    }

    auto Cmd::CmdFormatter::formatSuggestion(const std::size_t arrow_position) -> void
    {
        if (!suggestion.empty()) {
            fmt::format_to(std::back_inserter(formattingBuffer), "{:>6}", '|');
            fmt::format_to(
                std::back_inserter(formattingBuffer), "{1:>{0}} {2}\n",
                std::max<std::size_t>(1, arrow_position), "Suggestion: ", suggestion);
        }
    }
}// namespace ccl::handler

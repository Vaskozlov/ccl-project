#include <ccl/handler/cmd_handler.hpp>

namespace ccl::handler
{
    auto Cmd::instance() -> Cmd &
    {
        static auto cmd_handler = Cmd{};
        return cmd_handler;
    }

    auto Cmd::handleError(const ExceptionT *error) -> void
    {
        formatAndPrint<::fmt::color::red>(error, " error: ");
    }

    auto Cmd::handleWarning(const ExceptionT *warning) -> void
    {
        formatAndPrint<::fmt::color::medium_violet_red>(warning, " warning: ");
    }

    auto Cmd::handleSuggestion(const ExceptionT *suggestion) -> void
    {
        formatAndPrint<::fmt::color::white>(suggestion, " suggestion: ");
    }

    template<::fmt::color Color, typename HandleType>
    auto Cmd::formatAndPrint(const ExceptionT *value, HandleType &&handle_type) -> void
    {
        auto line = value->getLine();
        auto column = value->getColumn();
        auto message = value->getMessage();
        auto suggestion = value->getSuggestion();
        const auto &filename = value->getFilename();
        const auto &working_line = value->getWorkingLine();

        auto result = std::string{};

        ::fmt::format_to(
            std::back_inserter(result),
            ::fmt::emphasis::underline | ::fmt::fg(::fmt::color::cornflower_blue), "{}:{}:{}",
            filename, line, column);

        ::fmt::format_to(std::back_inserter(result), ::fmt::fg(::fmt::color::cornflower_blue), ":");

        ::fmt::format_to(std::back_inserter(result), ::fmt::fg(Color), handle_type);
        ::fmt::format_to(std::back_inserter(result), "{}\n", message);

        ::fmt::format_to(std::back_inserter(result), ::fmt::emphasis::italic, "{}\n", working_line);
        ::fmt::format_to(std::back_inserter(result), "{:>{}}\n", "^", column);

        if (value->hasSuggestion()) {
            ::fmt::format_to(std::back_inserter(result), "suggestion: {}\n", suggestion);
        }

        ::fmt::print("{}", result);
        std::cout.flush();
    }
}// namespace ccl::handler

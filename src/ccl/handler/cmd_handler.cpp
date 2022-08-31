#include <ccl/handler/cmd_handler.hpp>

namespace ccl::handler
{
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
        auto line = value->getLine();
        auto column = value->getColumn();
        auto message = value->getMessage();
        auto suggestion = value->getSuggestion();
        const auto &filename = value->getFilename();
        const auto &working_line = value->getWorkingLine();

        auto result = std::string{};

        fmt::format_to(
            std::back_inserter(result),
            fmt::emphasis::underline | fmt::fg(fmt::color::cornflower_blue), "{}:{}:{}", filename,
            line, column);

        fmt::format_to(std::back_inserter(result), fmt::fg(fmt::color::cornflower_blue), ":");

        fmt::format_to(std::back_inserter(result), fmt::fg(Color), handle_type);
        fmt::format_to(std::back_inserter(result), "{}\n", message);

        fmt::format_to(std::back_inserter(result), fmt::emphasis::italic, "{}\n", working_line);
        fmt::format_to(
            std::back_inserter(result), fmt::fg(fmt::color::light_green), "{:>{}}\n", "^", column);

        if (value->hasSuggestion()) {
            fmt::format_to(std::back_inserter(result), "Suggest to {}\n", suggestion);
        }

        fmt::print("{}\n", result);
        std::cout.flush();
    }
}// namespace ccl::handler

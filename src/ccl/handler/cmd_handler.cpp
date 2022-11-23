#include <ccl/handler/cmd_handler.hpp>

namespace ccl::handler
{
    Cmd Cmd::defaultCmdHandler;

    Cmd::CmdFormatter::CmdFormatter(const ExceptionT *exception) noexcept
      : working_line{exception->getWorkingLine()}
      , filename{exception->getFilename()}
      , message{exception->getMessage()}
      , suggestion{exception->getSuggestion()}
      , length{exception->getLength()}
      , line{exception->getLine()}
      , column{exception->getColumn()}
    {}

    template<fmt::color Color, typename HandleType>
    auto Cmd::CmdFormatter::formatErrorCategory(HandleType &&handle_type) -> void
    {
        format_to(std::back_inserter(formatting_buffer), fg(Color), handle_type);
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
        auto formatter = CmdFormatter{value};
        fmt::print("{}\n", formatter.format<Color>(std::forward<HandleType>(handle_type)));
        std::cout.flush();
    }
}// namespace ccl::handler

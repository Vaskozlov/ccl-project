#include <ccl/handler/cmd.hpp>

namespace ccl::handler
{
    auto Cmd::instance() -> Cmd &
    {
        static auto default_instance = Cmd{};

        return default_instance;
    }

    Cmd::CmdFormatter::CmdFormatter(const ExceptionT *exception) noexcept
      : workingLine{exception->getWorkingLine()}
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
        format_to(
            std::back_inserter(formattingBuffer), fg(Color), std::forward<HandleType>(handle_type));
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

        return formattingBuffer;
    }

    auto Cmd::onHandle(const ExceptionT *error) -> void
    {
        switch (error->getCriticality()) {
        case ExceptionCriticality::SUGGESTION:
            formatAndPrint<fmt::color::white>(error, " suggestion: ");
            break;

        case ExceptionCriticality::WARNING:
            formatAndPrint<fmt::color::medium_violet_red>(error, " warning: ");
            break;

        default:
            formatAndPrint<fmt::color::red>(error, " error: ");
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

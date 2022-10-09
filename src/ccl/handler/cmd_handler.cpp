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
        auto formatter = CmdFormatter{ value };
        fmt::print("{}\n", formatter.format<Color>(std::forward<HandleType>(handle_type)));
        std::cout.flush();
    }

    Cmd::CmdFormatter::CmdFormatter(const ExceptionT *exception) noexcept
      : working_line(exception->getWorkingLine()), filename(exception->getFilename()),
        message(exception->getMessage()), suggestion(exception->getSuggestion()),
        length(exception->getLength()), line(exception->getLine()), column(exception->getColumn())
    {}
}// namespace ccl::handler

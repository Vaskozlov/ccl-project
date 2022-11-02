#include <ccl/handler/cmd_handler.hpp>
#include <ccl/handler/exception_handler.hpp>

namespace ccl
{
    ExceptionHandler ExceptionHandler::defaultExceptionHandler;

    auto ExceptionHandler::handle(const ExceptionT *const error) -> void
    {
        switch (error->getCriticality()) {
        case ExceptionCriticality::SUGGESTION:
            ++suggestion_count;
            break;

        case ExceptionCriticality::WARNING:
            ++warnings_count;
            break;

        case ExceptionCriticality::UNCRITICAL:
            ++uncritical_errors_count;
            break;

        case ExceptionCriticality::CRITICAL:
            ++critical_errors_count;
            break;

        case ExceptionCriticality::PANIC:
            ++panic_error_count;
            break;

        default:
            std::unreachable();
        }

        onHandle(error);
    }

    // NOLINTNEXTLINE recursive call
    auto ExceptionHandler::onHandle(const ExceptionT *const error) -> void
    {
        static auto cmd_handler = handler::Cmd{};

        if (error->getCriticality() >= ExceptionCriticality::CRITICAL) {
            throw ExceptionT(*error);
        }

        cmd_handler.handle(error);
    }

}// namespace ccl

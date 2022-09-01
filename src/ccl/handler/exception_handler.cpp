#include <ccl/handler/cmd_handler.hpp>
#include <ccl/handler/exception_handler.hpp>

namespace ccl
{
    auto ExceptionHandler::instance() -> ExceptionHandler &
    {
        static auto handler = ExceptionHandler{};
        return handler;
    }

    auto ExceptionHandler::handle(const ExceptionT *error) -> void
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
            throw InvalidArgument{ "Invalid criticality" };
        }

        onHandle(error);
    }

    // NOLINTNEXTLINE recursive call
    auto ExceptionHandler::onHandle(const ExceptionT *error) -> void
    {
        static auto cmd_handler = handler::Cmd{};

        if (error->getCriticality() >= ExceptionCriticality::CRITICAL) {
            throw ExceptionT(*error);
        }

        cmd_handler.handle(error);
    }

}// namespace ccl

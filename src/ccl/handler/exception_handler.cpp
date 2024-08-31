#include <atomic>
#include <ccl/handler/cmd.hpp>
#include <ccl/handler/exception_handler.hpp>

namespace ccl
{
    auto ExceptionHandler::instance() -> ExceptionHandler &
    {
        static auto default_instance = ExceptionHandler{};

        return default_instance;
    }

    // NOLINTNEXTLINE
    auto ExceptionHandler::handle(const ExceptionT *const error) const -> void
    {
        switch (error->getCriticality()) {
        case ExceptionCriticality::SUGGESTION:
            suggestionsCounter.fetch_add(1, std::memory_order_relaxed);
            break;

        case ExceptionCriticality::WARNING:
            warningsCounter.fetch_add(1, std::memory_order_relaxed);
            break;

        case ExceptionCriticality::UNCRITICAL:
            uncriticalErrorsCounter.fetch_add(1, std::memory_order_relaxed);
            break;

        case ExceptionCriticality::CRITICAL:
            criticalErrorsCounter.fetch_add(1, std::memory_order_relaxed);
            break;

        case ExceptionCriticality::PANIC:
            panicErrorsCounter.fetch_add(1, std::memory_order_relaxed);
            break;

        default:
            isl::unreachable();
        }

        onHandle(error);
    }

    // NOLINTNEXTLINE recursive call
    auto ExceptionHandler::onHandle(const ExceptionT *const error) const -> void
    {
        const auto &cmd_handler = handler::Cmd::instance();

        if (error->getCriticality() >= ExceptionCriticality::CRITICAL) {
            throw ExceptionT(*error);
        }

        cmd_handler.handle(error);
    }
}// namespace ccl

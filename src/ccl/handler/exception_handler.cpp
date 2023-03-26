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
    auto ExceptionHandler::handle(const ExceptionT *const error) -> void
    {
        switch (error->getCriticality()) {
        case ExceptionCriticality::SUGGESTION:
            std::atomic_fetch_add_explicit(&suggestionsCounter, 1UL, std::memory_order_relaxed);
            break;

        case ExceptionCriticality::WARNING:
            std::atomic_fetch_add_explicit(&warningsCounter, 1UL, std::memory_order_relaxed);
            break;

        case ExceptionCriticality::UNCRITICAL:
            std::atomic_fetch_add_explicit(
                &uncriticalErrorsCounter, 1UL, std::memory_order_relaxed);
            break;

        case ExceptionCriticality::CRITICAL:
            std::atomic_fetch_add_explicit(&criticalErrorsCounter, 1UL, std::memory_order_relaxed);
            break;

        case ExceptionCriticality::PANIC:
            std::atomic_fetch_add_explicit(&panicErrorsCounter, 1UL, std::memory_order_relaxed);
            break;

        default:
            std::unreachable();
        }

        onHandle(error);
    }

    // NOLINTNEXTLINE recursive call
    auto ExceptionHandler::onHandle(const ExceptionT *const error) -> void
    {
        auto &cmd_handler = handler::Cmd::instance();

        if (error->getCriticality() >= ExceptionCriticality::CRITICAL) {
            throw ExceptionT(*error);
        }

        cmd_handler.handle(error);
    }
}// namespace ccl

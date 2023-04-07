#include "ccl/text/text_iterator.hpp"

namespace ccl::text
{
    auto TextIterator::throwSuggestion(
        AnalysisStage stage, const TextIterator &iterator_location, string_view message,
        string_view suggestion) -> void
    {
        throwToHandle(
            iterator_location, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
    }

    auto TextIterator::throwWarning(
        AnalysisStage stage, const TextIterator &iterator_location, string_view message,
        string_view suggestion) -> void
    {
        throwToHandle(iterator_location, ExceptionCriticality::WARNING, stage, message, suggestion);
    }

    auto TextIterator::throwUncriticalError(
        AnalysisStage stage, const TextIterator &iterator_location, string_view message,
        string_view suggestion) -> void
    {
        throwToHandle(
            iterator_location, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
    }

    auto TextIterator::throwCriticalError(
        AnalysisStage stage, const TextIterator &iterator_location, string_view message,
        string_view suggestion) -> void
    {
        throwToHandle(
            iterator_location, ExceptionCriticality::CRITICAL, stage, message, suggestion);
    }

    auto TextIterator::throwPanicError(
        AnalysisStage stage, const TextIterator &iterator_location, string_view message,
        string_view suggestion) -> void
    {
        throwToHandle(iterator_location, ExceptionCriticality::PANIC, stage, message, suggestion);
    }

    auto TextIterator::throwSuggestion(
        AnalysisStage stage, string_view message, string_view suggestion) -> void
    {
        throwToHandle(*this, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
    }

    auto TextIterator::throwWarning(
        AnalysisStage stage, string_view message, string_view suggestion) -> void
    {
        throwToHandle(*this, ExceptionCriticality::WARNING, stage, message, suggestion);
    }

    auto TextIterator::throwUncriticalError(
        AnalysisStage stage, string_view message, string_view suggestion) -> void
    {
        throwToHandle(*this, ExceptionCriticality::UNCRITICAL, stage, message, suggestion);
    }

    auto TextIterator::throwCriticalError(
        AnalysisStage stage, string_view message, string_view suggestion) -> void
    {
        throwToHandle(*this, ExceptionCriticality::CRITICAL, stage, message, suggestion);
    }

    auto TextIterator::throwPanicError(
        AnalysisStage stage, string_view message, string_view suggestion) -> void
    {
        throwToHandle(*this, ExceptionCriticality::PANIC, stage, message, suggestion);
    }

    auto TextIterator::throwToHandle(
        const TextIterator &iterator_location, ExceptionCriticality criticality,
        AnalysisStage stage, string_view message, string_view suggestion) -> void
    {
        auto exception = TextIteratorException(
            criticality, stage, iterator_location.getLocation(), 1,
            iterator_location.getWorkingLine(), as<std::string>(message),
            as<std::string>(suggestion));

        if (nullptr == exceptionHandler) {
            throw std::move(exception);
        }

        exceptionHandler->handle(exception);
    }
}// namespace ccl::text
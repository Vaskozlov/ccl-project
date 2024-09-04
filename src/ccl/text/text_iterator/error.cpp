#include "ccl/text/text_iterator.hpp"

namespace ccl::text
{
    auto TextIterator::throwSuggestion(
        const AnalysisStage stage, const TextIterator &iterator_location,
        const isl::string_view message, const isl::string_view suggestion) const -> void
    {
        throwToHandle(
            iterator_location, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
    }

    auto TextIterator::throwWarning(
        const AnalysisStage stage, const TextIterator &iterator_location,
        const isl::string_view message, const isl::string_view suggestion) const -> void
    {
        throwToHandle(iterator_location, ExceptionCriticality::WARNING, stage, message, suggestion);
    }

    auto TextIterator::throwUncriticalError(
        const AnalysisStage stage, const TextIterator &iterator_location,
        const isl::string_view message, const isl::string_view suggestion) const -> void
    {
        throwToHandle(
            iterator_location, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
    }

    auto TextIterator::throwCriticalError(
        const AnalysisStage stage, const TextIterator &iterator_location,
        const isl::string_view message, const isl::string_view suggestion) const -> void
    {
        throwToHandle(
            iterator_location, ExceptionCriticality::CRITICAL, stage, message, suggestion);
    }

    auto TextIterator::throwPanicError(
        const AnalysisStage stage, const TextIterator &iterator_location,
        const isl::string_view message, const isl::string_view suggestion) const -> void
    {
        throwToHandle(iterator_location, ExceptionCriticality::PANIC, stage, message, suggestion);
    }

    auto TextIterator::throwSuggestion(
        const AnalysisStage stage, const isl::string_view message,
        const isl::string_view suggestion) const -> void
    {
        throwToHandle(*this, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
    }

    auto TextIterator::throwWarning(
        const AnalysisStage stage, const isl::string_view message,
        const isl::string_view suggestion) const -> void
    {
        throwToHandle(*this, ExceptionCriticality::WARNING, stage, message, suggestion);
    }

    auto TextIterator::throwUncriticalError(
        const AnalysisStage stage, const isl::string_view message,
        const isl::string_view suggestion) const -> void
    {
        throwToHandle(*this, ExceptionCriticality::UNCRITICAL, stage, message, suggestion);
    }

    auto TextIterator::throwCriticalError(
        const AnalysisStage stage, const isl::string_view message,
        const isl::string_view suggestion) const -> void
    {
        throwToHandle(*this, ExceptionCriticality::CRITICAL, stage, message, suggestion);
    }

    auto TextIterator::throwPanicError(
        const AnalysisStage stage, const isl::string_view message,
        const isl::string_view suggestion) const -> void
    {
        throwToHandle(*this, ExceptionCriticality::PANIC, stage, message, suggestion);
    }

    auto TextIterator::throwToHandle(
        const TextIterator &iterator_location, const ExceptionCriticality criticality,
        const AnalysisStage stage, const isl::string_view message,
        const isl::string_view suggestion) const -> void
    {
        auto exception = TextIteratorException{
            criticality,
            stage,
            iterator_location.getLocation(),
            static_cast<std::string>(inputInfo.filename),
            1,
            static_cast<std::string>(iterator_location.getCurrentLine()),
            static_cast<std::string>(message),
            static_cast<std::string>(suggestion),
        };

        if (exceptionHandler == nullptr) {
            throw std::move(exception);
        }

        exceptionHandler->handle(exception);
    }
}// namespace ccl::text
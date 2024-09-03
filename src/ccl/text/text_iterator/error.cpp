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

    auto TextIterator::linesOfFragment(
        const isl::string_view whole_input, const isl::string_view fragment) -> isl::string_view
    {
        const auto *front_it = std::next(fragment.begin(), -1);
        const auto *back_it = fragment.end();

        while (front_it >= whole_input.begin()) {
            if (*front_it == U'\n') {
                break;
            }

            front_it = std::next(front_it, -1);
        }

        while (back_it < whole_input.end()) {
            if (*back_it == U'\n') {
                break;
            }

            back_it = std::next(back_it);
        }

        return {std::next(front_it), back_it};
    }

}// namespace ccl::text
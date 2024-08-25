#include "ccl/text/text_iterator.hpp"

namespace ccl::text
{
    auto TextIterator::throwSuggestion(
        AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
        isl::string_view suggestion) -> void
    {
        throwToHandle(
            iterator_location, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
    }

    auto TextIterator::throwWarning(
        AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
        isl::string_view suggestion) -> void
    {
        throwToHandle(iterator_location, ExceptionCriticality::WARNING, stage, message, suggestion);
    }

    auto TextIterator::throwUncriticalError(
        AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
        isl::string_view suggestion) -> void
    {
        throwToHandle(
            iterator_location, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
    }

    auto TextIterator::throwCriticalError(
        AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
        isl::string_view suggestion) -> void
    {
        throwToHandle(
            iterator_location, ExceptionCriticality::CRITICAL, stage, message, suggestion);
    }

    auto TextIterator::throwPanicError(
        AnalysisStage stage, const TextIterator &iterator_location, isl::string_view message,
        isl::string_view suggestion) -> void
    {
        throwToHandle(iterator_location, ExceptionCriticality::PANIC, stage, message, suggestion);
    }

    auto TextIterator::throwSuggestion(
        AnalysisStage stage, isl::string_view message, isl::string_view suggestion) -> void
    {
        throwToHandle(*this, ExceptionCriticality::SUGGESTION, stage, message, suggestion);
    }

    auto TextIterator::throwWarning(
        AnalysisStage stage, isl::string_view message, isl::string_view suggestion) -> void
    {
        throwToHandle(*this, ExceptionCriticality::WARNING, stage, message, suggestion);
    }

    auto TextIterator::throwUncriticalError(
        AnalysisStage stage, isl::string_view message, isl::string_view suggestion) -> void
    {
        throwToHandle(*this, ExceptionCriticality::UNCRITICAL, stage, message, suggestion);
    }

    auto TextIterator::throwCriticalError(
        AnalysisStage stage, isl::string_view message, isl::string_view suggestion) -> void
    {
        throwToHandle(*this, ExceptionCriticality::CRITICAL, stage, message, suggestion);
    }

    auto TextIterator::throwPanicError(
        AnalysisStage stage, isl::string_view message, isl::string_view suggestion) -> void
    {
        throwToHandle(*this, ExceptionCriticality::PANIC, stage, message, suggestion);
    }

    auto TextIterator::throwToHandle(
        const TextIterator &iterator_location, ExceptionCriticality criticality,
        AnalysisStage stage, isl::string_view message, isl::string_view suggestion) -> void
    {
        auto exception = TextIteratorException(
            criticality, stage, iterator_location.getLocation(), 1, iterator_location.getCurrentLine(),
            as<std::string>(message), as<std::string>(suggestion));

        if (nullptr == exceptionHandler) {
            throw std::move(exception);
        }

        exceptionHandler->handle(exception);
    }

    auto TextIterator::linesOfFragment(isl::string_view whole_input, isl::string_view fragment)
        -> isl::string_view
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
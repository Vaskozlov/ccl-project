#ifndef CCL_PROJECT_EXCEPTION_HANDLER_HPP
#define CCL_PROJECT_EXCEPTION_HANDLER_HPP

#include <ccl/ccl.hpp>
#include <ccl/text/iterator_exception.hpp>

namespace ccl
{
    template<typename T>
    concept DerivedFromTextIteratorException = std::derived_from<T, text::TextIteratorException>;

    struct ExceptionHandler
    {
        using ExceptionT = text::TextIteratorException;

        ExceptionHandler() noexcept = default;
        ExceptionHandler(ExceptionHandler &&) noexcept = default;
        ExceptionHandler(const ExceptionHandler &) noexcept = default;

        virtual ~ExceptionHandler() = default;

        auto operator=(const ExceptionHandler &) -> ExceptionHandler & = default;
        auto operator=(ExceptionHandler &&) noexcept -> ExceptionHandler & = default;

        static auto instance() -> ExceptionHandler &
        {
            static auto handler = ExceptionHandler{};
            return handler;
        }

        virtual auto handleError(const ExceptionT *error) -> void
        {
            throw ExceptionT(*error);
        }

        virtual auto handleWarning(const ExceptionT * /* warning */) -> void
        {}

        virtual auto handleSuggestion(const ExceptionT * /* suggestion */) -> void
        {}

        template<DerivedFromTextIteratorException T>
        auto handleError(const T *error)
        {
            handleError(static_cast<const text::TextIteratorException *>(error));
        }

        template<DerivedFromTextIteratorException T>
        auto handleWarning(const T *warning) -> void
        {
            handleWarning(static_cast<const text::TextIteratorException *>(warning));
        }

        template<DerivedFromTextIteratorException T>
        auto handleSuggestion(const T *suggestion) -> void
        {
            handleSuggestion(static_cast<const text::TextIteratorException *>(suggestion));
        }

        template<DerivedFromTextIteratorException T>
        auto handleError(const T &error)
        {
            handleError(static_cast<const text::TextIteratorException *>(&error));
        }

        template<DerivedFromTextIteratorException T>
        auto handleWarning(const T &warning) -> void
        {
            handleWarning(static_cast<const text::TextIteratorException *>(&warning));
        }

        template<DerivedFromTextIteratorException T>
        auto handleSuggestion(const T &suggestion) -> void
        {
            handleSuggestion(static_cast<const text::TextIteratorException *>(&suggestion));
        }
    };
}// namespace ccl

#endif /* CCL_PROJECT_EXCEPTION_HANDLER_HPP */

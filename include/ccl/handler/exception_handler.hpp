#ifndef CCL_PROJECT_EXCEPTION_HANDLER_HPP
#define CCL_PROJECT_EXCEPTION_HANDLER_HPP

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

        static auto instance() -> ExceptionHandler &;

        auto handle(const ExceptionT *error) -> void;

        template<DerivedFromTextIteratorException T>
        auto handle(const T *error)
        {
            handle(static_cast<const ExceptionT *>(error));
        }

        template<DerivedFromTextIteratorException T>
        auto handle(const T &error)
        {
            handle(static_cast<const ExceptionT *>(&error));
        }

    private:
        virtual auto onHandle(const ExceptionT *error) -> void;

        size_t suggestion_count{};
        size_t warnings_count{};
        size_t uncritical_errors_count{};
        size_t critical_errors_count{};
        size_t panic_error_count{};
    };
}// namespace ccl

#endif /* CCL_PROJECT_EXCEPTION_HANDLER_HPP */

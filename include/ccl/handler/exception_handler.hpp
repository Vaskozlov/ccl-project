#ifndef CCL_PROJECT_EXCEPTION_HANDLER_HPP
#define CCL_PROJECT_EXCEPTION_HANDLER_HPP

#include <ccl/text/iterator_exception.hpp>

namespace ccl
{
    class ExceptionHandler
    {
    protected:
        using ExceptionT = text::TextIteratorException;

    private:
        static ExceptionHandler defaultExceptionHandler;

        size_t suggestion_count{};
        size_t warnings_count{};
        size_t uncritical_errors_count{};
        size_t critical_errors_count{};
        size_t panic_error_count{};

    public:
        ExceptionHandler() noexcept = default;
        ExceptionHandler(ExceptionHandler &&) noexcept = default;
        ExceptionHandler(const ExceptionHandler &) noexcept = default;

        virtual ~ExceptionHandler() = default;

        auto operator=(const ExceptionHandler &) -> ExceptionHandler & = default;
        auto operator=(ExceptionHandler &&) noexcept -> ExceptionHandler & = default;

        [[nodiscard]] static auto instance() -> ExceptionHandler &
        {
            return defaultExceptionHandler;
        }

        auto handle(const ExceptionT *error) -> void;

        template<std::derived_from<ExceptionT> T>
        auto handle(const T *error)
        {
            handle(as<const ExceptionT *>(error));
        }

        template<std::derived_from<ExceptionT> T>
        auto handle(const T &error)
        {
            handle(as<const ExceptionT *>(&error));
        }

    private:
        virtual auto onHandle(const ExceptionT *error) -> void;
    };
}// namespace ccl

#endif /* CCL_PROJECT_EXCEPTION_HANDLER_HPP */

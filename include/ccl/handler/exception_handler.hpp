#ifndef CCL_PROJECT_EXCEPTION_HANDLER_HPP
#define CCL_PROJECT_EXCEPTION_HANDLER_HPP

#include <atomic>
#include <ccl/text/iterator_exception.hpp>

namespace ccl
{
    class ExceptionHandler
    {
    protected:
        using ExceptionT = text::TextIteratorException;

    private:
        // NOLINTNEXTLINE static variable
        static ExceptionHandler defaultExceptionHandler;

        mutable std::atomic<std::size_t> suggestionsCounter;
        mutable std::atomic<std::size_t> warningsCounter;
        mutable std::atomic<std::size_t> uncriticalErrorsCounter;
        mutable std::atomic<std::size_t> criticalErrorsCounter;
        mutable std::atomic<std::size_t> panicErrorsCounter;

    public:
        ExceptionHandler() noexcept = default;

        ExceptionHandler(ExceptionHandler &&) = delete;
        ExceptionHandler(const ExceptionHandler &) = delete;

        virtual ~ExceptionHandler() = default;

        auto operator=(ExceptionHandler &&) -> void = delete;
        auto operator=(const ExceptionHandler &) -> void = delete;

        [[nodiscard]] static auto instance() -> ExceptionHandler &;

        auto handle(const ExceptionT *error) const -> void;

        template<std::derived_from<ExceptionT> T>
        auto handle(const T *error) const -> void
        {
            handle(as<const ExceptionT *>(error));
        }

        template<std::derived_from<ExceptionT> T>
        auto handle(const T &error) const -> void
        {
            handle(isl::as<const ExceptionT *>(&error));
        }

    private:
        virtual auto onHandle(const ExceptionT *error) const -> void;
    };
}// namespace ccl

#endif /* CCL_PROJECT_EXCEPTION_HANDLER_HPP */

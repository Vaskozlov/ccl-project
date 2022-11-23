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
        static ExceptionHandler defaultExceptionHandler;

        std::atomic<size_t> suggestionsCounter{};
        std::atomic<size_t> warningsCounter{};
        std::atomic<size_t> uncriticalErrorsCounter{};
        std::atomic<size_t> criticalErrorsCounter{};
        std::atomic<size_t> panicErrorsCounter{};

    public:
        ExceptionHandler() noexcept = default;
        virtual ~ExceptionHandler() = default;

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

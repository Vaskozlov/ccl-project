export module ccl.handler:excaption_handler;

export import isl;

export namespace ccl::text
{
    class TextIteratorException;
}

export namespace ccl::handler
{
    class ExceptionHandler
    {
    protected:
        using ExceptionT = text::TextIteratorException;

    private:
        static ExceptionHandler defaultExceptionHandler;// NOLINT

        std::atomic<std::size_t> suggestionsCounter;
        std::atomic<std::size_t> warningsCounter;
        std::atomic<std::size_t> uncriticalErrorsCounter;
        std::atomic<std::size_t> criticalErrorsCounter;
        std::atomic<std::size_t> panicErrorsCounter;

    public:
        ExceptionHandler() noexcept = default;

        ExceptionHandler(ExceptionHandler &&) = delete;
        ExceptionHandler(const ExceptionHandler &) = delete;

        virtual ~ExceptionHandler() = default;

        auto operator=(ExceptionHandler &&) -> void = delete;
        auto operator=(const ExceptionHandler &) -> void = delete;

        [[nodiscard]] static auto instance() -> ExceptionHandler &;

        auto handle(const ExceptionT *error) -> void;

        template<std::derived_from<ExceptionT> T>
        auto handle(const T *error) -> void
        {
            handle(as<const ExceptionT *>(error));
        }

        template<std::derived_from<ExceptionT> T>
        auto handle(const T &error) -> void
        {
            handle(isl::as<const ExceptionT *>(&error));
        }

    private:
        virtual auto onHandle(const ExceptionT *error) -> void;
    };
}// namespace ccl::handler

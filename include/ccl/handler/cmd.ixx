export module ccl.handler:cmd;

export import :excaption_handler;
export import :cmd_formatter;

export namespace ccl::handler
{
    class Cmd final : public ExceptionHandler
    {
    public:
        using ExceptionHandler::ExceptionT;

        Cmd() noexcept = default;

        [[nodiscard]] static auto instance() -> Cmd &;

    private:
        auto onHandle(const ExceptionT *error) -> void override;
    };
}// namespace ccl::handler

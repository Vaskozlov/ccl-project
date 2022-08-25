#ifndef CCL_PROJECT_EXCEPTION_HPP
#define CCL_PROJECT_EXCEPTION_HPP

#include <exception>
#include <string_view>

#define CCL_EXCEPTION(name, base_exception)                                                        \
    struct name : base_exception                                                                   \
    {                                                                                              \
        static_assert(std::is_base_of_v<ccl::CclException, base_exception>);                       \
                                                                                                   \
        name() = default;                                                                          \
                                                                                                   \
        template<typename... Ts>                                                                   \
        explicit name(Ts &&...args) : base_exception{ std::forward<Ts>(args)... }                  \
        {}                                                                                         \
    }

#define CCL_SAFE_VERSION                                                                           \
    template<UsageMode Mode = SAFE>                                                                \
    requires(Mode == SAFE)

#define CCL_UNSAFE_VERSION                                                                         \
    template<UsageMode Mode>                                                                       \
    requires(Mode == UNSAFE)

namespace ccl
{
    class CclException : public std::exception
    {
    public:
        [[nodiscard]] auto what() const noexcept -> const char * override
        {
            return message.data();
        }

        CclException() = default;

        explicit CclException(std::string_view message_) : message(message_)
        {}

    private:
        std::string_view message{};
    };

    enum UsageMode : bool
    {
        SAFE,
        UNSAFE
    };

    CCL_EXCEPTION(LogicError, CclException);
    CCL_EXCEPTION(OutOfRange, CclException);
    CCL_EXCEPTION(KeyNotFound, CclException);
    CCL_EXCEPTION(InvalidArgument, CclException);
    CCL_EXCEPTION(UnrecoverableError, CclException);
}// namespace ccl

#endif /* CCL_PROJECT_EXCEPTION_HPP */

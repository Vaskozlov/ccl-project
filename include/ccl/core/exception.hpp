#ifndef CCL_PROJECT_EXCEPTION_HPP
#define CCL_PROJECT_EXCEPTION_HPP

#include <exception>
#include <stdexcept>
#include <string_view>

#define CCL_EXCEPTION(name, base_exception)                                                        \
    struct name : base_exception                                                                   \
    {                                                                                              \
        template<typename... Ts>                                                                   \
            requires std::constructible_from<base_exception, Ts...>                                \
        explicit name(Ts &&...args)                                                                \
          : base_exception{std::forward<Ts>(args)...}                                              \
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
    enum UsageMode : bool
    {
        SAFE,
        UNSAFE
    };

    CCL_EXCEPTION(UnrecoverableError, std::runtime_error);
}// namespace ccl

#endif /* CCL_PROJECT_EXCEPTION_HPP */

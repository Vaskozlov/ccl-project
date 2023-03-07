#ifndef CCL_PROJECT_FUTURE_HPP
#define CCL_PROJECT_FUTURE_HPP

#include <ccl/ccl.hpp>
#include <future>

namespace ccl
{
    template<typename Function, typename... Ts>
    [[nodiscard]] CCL_INLINE auto async(Function &&function, Ts &&...args) -> decltype(auto)
    {
        return std::async(
            std::launch::async, std::forward<Function>(function), std::forward<Ts>(args)...);
    }

}// namespace ccl

#endif /* CCL_PROJECT_FUTURE_HPP */

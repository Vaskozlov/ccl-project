#ifndef CCL_PROJECT_LOGICAL_OPS_HPP
#define CCL_PROJECT_LOGICAL_OPS_HPP

#include <ccl/core/concepts.hpp>
#include <ccl/core/defines.hpp>
#include <utility>

namespace ccl
{
    template<Boolean... Ts>
    CCL_DECL auto land(Ts &&...args) noexcept -> bool
    {
        return (... && std::forward<Ts>(args));
    }

    template<Boolean... Ts>
    CCL_DECL auto lor(Ts &&...args) noexcept -> bool
    {
        return (... || std::forward<Ts>(args));
    }
}// namespace ccl

#endif /* CCL_PROJECT_LOGICAL_OPS_HPP */

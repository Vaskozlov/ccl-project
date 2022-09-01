#ifndef CCL_PROJECT_LOGICAL_OPS_HPP
#define CCL_PROJECT_LOGICAL_OPS_HPP

#include <ccl/core/concepts.hpp>
#include <ccl/core/defines.hpp>
#include <utility>

namespace ccl
{
    CCL_DECL auto land(Boolean auto &&...args) noexcept -> bool
    {
        return (... && args);
    }

    CCL_DECL auto lor(Boolean auto &&...args) noexcept -> bool
    {
        return (... || args);
    }
}// namespace ccl

#endif /* CCL_PROJECT_LOGICAL_OPS_HPP */

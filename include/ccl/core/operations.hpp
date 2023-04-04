#ifndef CCL_PROJECT_OPERATIONS_HPP
#define CCL_PROJECT_OPERATIONS_HPP

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

    template<Integral Int>
    CCL_DECL auto pow2(Int value) -> Int
    {
        return as<Int>(as<Int>(1) << value);
    }
}// namespace ccl

#endif /* CCL_PROJECT_OPERATIONS_HPP */

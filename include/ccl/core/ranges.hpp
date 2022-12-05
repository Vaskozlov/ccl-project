#ifndef CCL_PROJECT_RANGES_HPP
#define CCL_PROJECT_RANGES_HPP

#include <ccl/core/defines.hpp>
#include <ranges>

namespace ccl::views
{
    CCL_DECL auto dropBack(const auto &container, size_t elements_to_drop = 1)
    {
        elements_to_drop = std::min(elements_to_drop, std::size(container));
        return std::views::take(container.size() - elements_to_drop);
    }
}// namespace ccl::views

#endif /* CCL_PROJECT_RANGES_HPP */

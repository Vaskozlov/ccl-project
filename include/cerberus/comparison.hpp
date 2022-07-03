#ifndef CERBERUS_PROJECT_COMPARISON_HPP
#define CERBERUS_PROJECT_COMPARISON_HPP

#include <cerberus/cerberus.hpp>
#include <cmath>

namespace cerb
{
    template<std::equality_comparable T>
    CERBLIB_DECL auto safeEqual(const T &lhs, const T &rhs) -> bool
    {
        if constexpr (std::floating_point<T>) {
            return std::abs(lhs - rhs) <= std::numeric_limits<T>::epsilon();
        } else {
            return lhs == rhs;
        }
    }

    template<std::equality_comparable T>
    CERBLIB_DECL auto safeNotEqual(const T &lhs, const T &rhs) -> bool
    {
        if constexpr (std::floating_point<T>) {
            return std::abs(lhs - rhs) > std::numeric_limits<T>::epsilon();
        } else {
            return lhs != rhs;
        }
    }
}// namespace cerb

#endif /* CERBERUS_PROJECT_COMPARISON_HPP */

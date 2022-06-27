#ifndef CERBERUS_PROJECT_COMPARISON_HPP
#define CERBERUS_PROJECT_COMPARISON_HPP

#include <cmath>
#include <cerberus/cerberus.hpp>

namespace cerb
{
    template<typename T>
    CERBLIB_DECL auto safeEqual(T const &lhs, T const &rhs) -> bool
    {
        if constexpr (std::floating_point<T>) {
            return std::abs(lhs - rhs) <= std::numeric_limits<T>::epsilon();
        } else {
            return lhs == rhs;
        }
    }

    template<typename T>
    CERBLIB_DECL auto safeNotEqual(T const &lhs, T const &rhs) -> bool
    {
        if constexpr (std::floating_point<T>) {
            return std::abs(lhs - rhs) > std::numeric_limits<T>::epsilon();
        } else {
            return lhs != rhs;
        }
    }
}// namespace cerb

#endif /* CERBERUS_PROJECT_COMPARISON_HPP */

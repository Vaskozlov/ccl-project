#ifndef CERBERUS_PROJECT_COMPARISON_HPP
#define CERBERUS_PROJECT_COMPARISON_HPP

#include <cerberus/cerberus.hpp>
#include <cmath>

namespace cerb
{
    template<typename T, typename U>
    CERBLIB_DECL auto safeEqual(const T &lhs, const U &rhs) -> bool
    {
        if constexpr (std::floating_point<T>) {
            return std::abs(lhs - static_cast<T>(rhs)) <= std::numeric_limits<T>::epsilon();
        } else {
            return lhs == rhs;
        }
    }

    template<typename T, typename U>
    CERBLIB_DECL auto safeNotEqual(const T &lhs, const U &rhs) -> bool
    {
        if constexpr (std::floating_point<T>) {
            return std::abs(lhs - static_cast<T>(rhs)) > std::numeric_limits<T>::epsilon();
        } else {
            return lhs != rhs;
        }
    }
}// namespace cerb

#endif /* CERBERUS_PROJECT_COMPARISON_HPP */

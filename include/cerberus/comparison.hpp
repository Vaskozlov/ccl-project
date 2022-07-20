#ifndef CERBERUS_PROJECT_COMPARISON_HPP
#define CERBERUS_PROJECT_COMPARISON_HPP

#include <cerberus/cerberus.hpp>
#include <cmath>

namespace cerb
{
    template<typename T1, typename T2>
    CERBLIB_DECL auto safeEqual(const T1 &lhs, const T2 &rhs) -> bool
    {
        constexpr auto rhs_is_floating_point =
            not std::floating_point<T1> and std::floating_point<T2>;

        constexpr auto rhs_is_bigger_floating_point =
            std::floating_point<T1> && std::floating_point<T2> && (sizeof(T2) > sizeof(T1));

        constexpr auto need_to_swap_arguments =
            rhs_is_floating_point || rhs_is_bigger_floating_point;

        if constexpr (need_to_swap_arguments) {
            return safeEqual(rhs, lhs);
        } else if constexpr (std::floating_point<T1>) {
            return std::abs(lhs - static_cast<T1>(rhs)) <= std::numeric_limits<T1>::epsilon();
        } else {
            return lhs == rhs;
        }
    }

    template<typename T1, typename T2>
    CERBLIB_DECL auto safeNotEqual(const T1 &lhs, const T2 &rhs) -> bool
    {
        constexpr auto rhs_is_floating_point =
            not std::floating_point<T1> and std::floating_point<T2>;

        constexpr auto rhs_is_bigger_floating_point =
            std::floating_point<T1> && std::floating_point<T2> && (sizeof(T2) > sizeof(T1));

        constexpr auto need_to_swap_arguments =
            rhs_is_floating_point || rhs_is_bigger_floating_point;

        if constexpr (need_to_swap_arguments) {
            return safeNotEqual(rhs, lhs);
        } else if constexpr (std::floating_point<T1>) {
            return std::abs(lhs - static_cast<T1>(rhs)) > std::numeric_limits<T1>::epsilon();
        } else {
            return lhs != rhs;
        }
    }
}// namespace cerb

#endif /* CERBERUS_PROJECT_COMPARISON_HPP */

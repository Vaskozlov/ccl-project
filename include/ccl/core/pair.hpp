#ifndef CCL_PROJECT_PAIR_HPP
#define CCL_PROJECT_PAIR_HPP

#include <ccl/ccl.hpp>

namespace ccl
{
    template<typename T1, typename T2>
    struct CCL_TRIVIAL_ABI TrivialPair
    {
        static_assert(std::is_trivial_v<T1> && std::is_trivial_v<T2>);

        TrivialPair() = default;

        constexpr TrivialPair(T1 first_, T2 second_) noexcept : first(first_), second(second_)
        {}

        constexpr auto operator<=>(const TrivialPair &other) const noexcept
            -> std::weak_ordering = default;

        T1 first;
        T2 second;
    };

    template<typename T1, typename T2>
    using Pair = std::conditional_t<
        std::is_trivial_v<T1> && std::is_trivial_v<T2>, TrivialPair<T1, T2>, std::pair<T1, T2>>;
}// namespace ccl

#endif /* CCL_PROJECT_PAIR_HPP */

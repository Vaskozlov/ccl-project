#ifndef CERBERUS_PROJECT_PAIR_HPP
#define CERBERUS_PROJECT_PAIR_HPP

#include <cerberus/cerberus.hpp>
#include <cerberus/comparison.hpp>

namespace cerb
{
    enum struct PairComparison
    {
        DEFAULT,
        FIRST_VALUE,
        SECOND_VALUE
    };

    template<typename T1, typename T2, PairComparison Comparison = PairComparison::DEFAULT>
    struct CERBLIB_TRIVIAL_ABI Pair
    {
        template<Pairable Pair>
        constexpr auto operator==(Pair const &other) const
            -> bool requires(Comparison == PairComparison::FIRST_VALUE)
        {
            return safeEqual(first, other.first);
        }

        template<Pairable Pair>
        constexpr auto operator==(Pair const &other) const
            -> bool requires(Comparison == PairComparison::SECOND_VALUE)
        {
            return safeEqual(second, other.second);
        }

        template<Pairable Pair>
        constexpr auto operator==(Pair const &other) const
            -> bool requires(Comparison == PairComparison::DEFAULT)
        {
            return safeEqual(first, other.first) && safeEqual(second, other.second);
        }

        template<Pairable Pair>
        constexpr auto operator<=>(Pair const &other) const -> std::strong_ordering
            requires(Comparison == PairComparison::FIRST_VALUE)
        {
            return first <=> other.first;
        }

        template<Pairable Pair>
        constexpr auto operator<=>(Pair const &other) const -> std::strong_ordering
            requires(Comparison == PairComparison::SECOND_VALUE)
        {
            return second <=> other.second;
        }

        template<Pairable Pair>
        constexpr auto operator<=>(Pair const &other) const -> std::strong_ordering
            requires(Comparison == PairComparison::DEFAULT)
        {
            if (safeEqual(first, other.first)) {
                return second <=> other.second;
            }

            return first <=> other.first;
        }

        Pair() = default;

        constexpr explicit Pair(T1 &&first_) : first(std::move(first_))
        {}

        constexpr explicit Pair(T1 const &first_) : first(first_)
        {}

        constexpr Pair(T1 &&first_, T2 &&second_)
          : first(std::move(first_)), second(std::move(second_))
        {}

        constexpr Pair(T1 const &first_, T2 const &second_) : first(first_), second(second_)
        {}

        T1 first{};
        T2 second{};
    };
}// namespace cerb

#endif /* CERBERUS_PROJECT_PAIR_HPP */

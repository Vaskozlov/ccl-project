#ifndef CCL_PROJECT_RANGE_HPP
#define CCL_PROJECT_RANGE_HPP

#include <ccl/core/iterator.hpp>
#include <ccl/core/pair.hpp>

namespace ccl
{
    template<std::unsigned_integral T = size_t>
    class Range : public AutoIteratorMethods<Range<T>>
    {
    public:
        class iterator
        {
        private:
            T value;

        public:
            explicit constexpr iterator(T t_value)
              : value{t_value}
            {}

            CCL_DECL auto operator*() const noexcept -> T
            {
                return value;
            }

            constexpr auto operator++() noexcept CCL_LIFETIMEBOUND->iterator &
            {
                ++value;
                return *this;
            }

            constexpr auto operator++(int) noexcept CCL_LIFETIMEBOUND->iterator &
            {
                auto old = *this;
                ++value;
                return old;
            }

            CCL_DECL auto operator<=>(const iterator &) const noexcept
                -> std::weak_ordering = default;
        };

        T from;
        T to;

        Range() = default;

        CCL_DECL Range(T t_from, T t_to)
          : from{t_from}
          , to{t_to}
        {}

        CCL_DECL explicit Range(Pair<T, T> range)
          : from{range.first}
          , to{range.second}
        {}

        CCL_DECL auto begin() const noexcept -> iterator
        {
            return iterator{from};
        }

        CCL_DECL auto end() const noexcept -> iterator
        {
            return iterator{to};
        }

        CCL_DECL auto inRange(T value) const noexcept
        {
            return land(value >= from, value <= to);
        }
    };
}// namespace ccl

#endif /* CCL_PROJECT_RANGE_HPP */

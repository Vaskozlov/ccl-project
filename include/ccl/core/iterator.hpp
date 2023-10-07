#ifndef CCL_PROJECT_ITERATOR_HPP
#define CCL_PROJECT_ITERATOR_HPP

#include <ccl/core/defines.hpp>
#include <iterator>

namespace ccl
{
    template<typename T>
    class AutoIteratorMethods
    {
        CCL_DECL auto toParent() const noexcept CCL_LIFETIMEBOUND->const T &
        {
            return static_cast<const T &>(*this);
        }

        CCL_DECL auto begin() const
            CCL_NOEXCEPT_IF(std::declval<const T>().begin()) CCL_LIFETIMEBOUND->decltype(auto)
        {
            return std::begin(toParent());
        }

        CCL_DECL auto end() const
            CCL_NOEXCEPT_IF(std::declval<const T>().end()) CCL_LIFETIMEBOUND->decltype(auto)
        {
            return std::end(toParent());
        }

    public:
        CCL_DECL auto cbegin() const CCL_NOEXCEPT_IF(begin()) CCL_LIFETIMEBOUND->decltype(auto)
        {
            return begin();
        }

        CCL_DECL auto cend() const CCL_NOEXCEPT_IF(end()) CCL_LIFETIMEBOUND->decltype(auto)
        {
            return end();
        }

        CCL_DECL auto rbegin() CCL_NOEXCEPT_IF(end()) CCL_LIFETIMEBOUND->decltype(auto)
        {
            return std::reverse_iterator{end()};
        }

        CCL_DECL auto rend() CCL_NOEXCEPT_IF(begin()) CCL_LIFETIMEBOUND->decltype(auto)
        {
            return std::reverse_iterator{begin()};
        }

        CCL_DECL auto rbegin() const CCL_NOEXCEPT_IF(end()) CCL_LIFETIMEBOUND->decltype(auto)
        {
            return std::reverse_iterator{end()};
        }

        CCL_DECL auto rend() const CCL_NOEXCEPT_IF(begin()) CCL_LIFETIMEBOUND->decltype(auto)
        {
            return std::reverse_iterator{begin()};
        }

        CCL_DECL auto crbegin() const CCL_NOEXCEPT_IF(rbegin()) CCL_LIFETIMEBOUND->decltype(auto)
        {
            return rbegin();
        }

        CCL_DECL auto crend() const CCL_NOEXCEPT_IF(rend()) CCL_LIFETIMEBOUND->decltype(auto)
        {
            return rend();
        }
    };
}// namespace ccl

#endif /* CCL_PROJECT_ITERATOR_HPP */

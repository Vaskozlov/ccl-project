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

        CCL_DECL auto begin() const noexcept(noexcept(std::declval<const T>().begin()))
            -> decltype(auto)
        {
            return std::begin(toParent());
        }

        CCL_DECL auto end() const noexcept(noexcept(std::declval<const T>().end()))
            -> decltype(auto)
        {
            return std::end(toParent());
        }

    public:
        CCL_DECL auto cbegin() const noexcept(noexcept(begin())) -> decltype(auto)
        {
            return begin();
        }

        CCL_DECL auto cend() const noexcept(noexcept(end())) -> decltype(auto)
        {
            return end();
        }

        CCL_DECL auto rbegin() noexcept(noexcept(end())) -> decltype(auto)
        {
            return std::reverse_iterator{end()};
        }

        CCL_DECL auto rend() noexcept(noexcept(begin())) -> decltype(auto)
        {
            return std::reverse_iterator{begin()};
        }

        CCL_DECL auto rbegin() const noexcept(noexcept(end())) -> decltype(auto)
        {
            return std::reverse_iterator{end()};
        }

        CCL_DECL auto rend() const noexcept(noexcept(begin())) -> decltype(auto)
        {
            return std::reverse_iterator{begin()};
        }

        CCL_DECL auto crbegin() const noexcept(noexcept(rbegin())) -> decltype(auto)
        {
            return rbegin();
        }

        CCL_DECL auto crend() const noexcept(noexcept(rend())) -> decltype(auto)
        {
            return rend();
        }
    };
}// namespace ccl

#endif /* CCL_PROJECT_ITERATOR_HPP */

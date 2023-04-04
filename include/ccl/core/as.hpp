#ifndef CCL_PROJECT_AS_HPP
#define CCL_PROJECT_AS_HPP

#include <ccl/core/defines.hpp>
#include <type_traits>
#include <utility>

namespace ccl
{
    template<typename T, typename U>
    CCL_DECL auto as(U &&value) -> T
    {
        return static_cast<T>(std::forward<U>(value));
    }

    template<typename T, typename U>
    CCL_DECL auto as(U &value) -> T
        requires std::is_reference_v<T> && std::derived_from<U, std::remove_reference_t<T>>
    {
        return static_cast<T>(value);
    }

    template<typename T, typename U>
    CCL_DECL auto as(U &value) -> T
        requires std::is_reference_v<T> && (!std::derived_from<U, std::remove_reference_t<T>>)
    {
        return dynamic_cast<T>(value);
    }

    template<typename T, typename U>
    CCL_DECL auto as(U *value) -> T
        requires std::is_pointer_v<T> && std::derived_from<U, std::remove_pointer_t<T>>
    {
        return static_cast<T>(value);
    }

    template<typename T, typename U>
    CCL_DECL auto as(U *value) -> T
        requires std::is_pointer_v<T> && (!std::derived_from<U, std::remove_pointer_t<T>>)
    {
        return dynamic_cast<T>(value);
    }
}// namespace ccl

#endif /* CCL_PROJECT_AS_HPP */

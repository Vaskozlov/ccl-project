#ifndef CCL_PROJECT_JOIN_HPP
#define CCL_PROJECT_JOIN_HPP

#include <ccl/string_view.hpp>

namespace ccl
{
    CCL_DECL auto join(
        Iterable auto &&container, Invocable<decltype(*container.begin())> auto &&function,
        StringLike<char> auto &&separator) -> std::string
    {
        auto result = std::string{};
        auto begin = std::begin(container);
        const auto end = std::end(container);
        const auto converted_separator = as<std::string_view>(separator);

        if (begin == end) {
            return result;
        }

        result.append(function(*begin));

        for (++begin; begin != end; ++begin) {
            result.append(converted_separator);
            result.append(function(*begin));
        }

        return result;
    }

    CCL_DECL CCL_INLINE auto join(
        Iterable auto &&container, Invocable<decltype(*container.begin())> auto &&function,
        string_view &&separator) -> std::string
    {
        return join(
            std::forward<decltype(container)>(container),
            std::forward<decltype(function)>(function), as<std::string_view>(separator));
    }

    CCL_DECL auto join(Iterable auto &&container, StringLike<char> auto &&separator) -> std::string
    {
        if constexpr (StringLike<decltype(*container.begin()), char>) {
            return join(
                std::forward<decltype(container)>(container),
                [](auto &&elem) -> decltype(auto) {
                    return elem;
                },
                std::forward<decltype(separator)>(separator));
        } else {
            return join(
                std::forward<decltype(container)>(container),
                [](auto &&elem) -> decltype(auto) {
                    return fmt::to_string(elem);
                },
                std::forward<decltype(separator)>(separator));
        }
    }

    CCL_DECL CCL_INLINE auto join(Iterable auto &&container, string_view &&separator) -> std::string
    {
        return join(std::forward<decltype(container)>(container), as<std::string_view>(separator));
    }
}// namespace ccl

#endif /* CCL_PROJECT_JOIN_HPP */

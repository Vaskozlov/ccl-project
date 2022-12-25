#ifndef CCL_PROJECT_CONCEPTS_HPP
#define CCL_PROJECT_CONCEPTS_HPP

#include <concepts>
#include <exception>

namespace ccl
{
    template<typename T>
    concept Enum = std::is_enum_v<T>;

    template<typename T>
    concept Boolean = std::is_convertible_v<T, bool>;

    template<typename T>
    concept Exception = std::is_base_of_v<std::exception, T>;

    template<typename T, typename... Ts>
    concept IsSameToAny = (... || std::is_same_v<T, Ts>);

    template<typename T>
    concept CharacterLiteral = IsSameToAny<T, char, char8_t, char16_t, char32_t, wchar_t>;

    template<typename T>
    concept CharacterArray = std::is_pointer_v<T> || std::is_array_v<T>;
}// namespace ccl

#endif /* CCL_PROJECT_CONCEPTS_HPP */

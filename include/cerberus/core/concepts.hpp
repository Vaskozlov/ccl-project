#ifndef CERBERUS_PROJECT_CONCEPTS_HPP
#define CERBERUS_PROJECT_CONCEPTS_HPP

#include <concepts>
#include <execution>

namespace cerb
{
    template<typename T>
    concept Enum = std::is_enum_v<T>;

    template<typename T>
    concept Boolean = std::is_same_v<T, bool>;

    template<typename T>
    concept Exception = std::is_base_of_v<std::exception, T>;

    template<typename T>
    concept CharacterLiteral =
        std::is_same_v<T, char> || std::is_same_v<T, unsigned char> || std::is_same_v<T, char8_t> ||
        std::is_same_v<T, char16_t> || std::is_same_v<T, char32_t> || std::is_same_v<T, wchar_t>;

    template<typename T>
    concept Pairable = requires(T pair) {
                           pair.first;
                           pair.second;
                       };
}// namespace cerb

#endif /* CERBERUS_PROJECT_CONCEPTS_HPP */

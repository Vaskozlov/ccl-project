#ifndef CERBERUS_PROJECT_ENUM_HPP
#define CERBERUS_PROJECT_ENUM_HPP

#include <cerberus/core/defines.hpp>

#define CERBLIB_ENUM(Name, Type, ...)                                                              \
    struct Name                                                                                    \
    {                                                                                              \
        CERBLIB_DECL operator Type() const                                                         \
        {                                                                                          \
            return value;                                                                          \
        }                                                                                          \
                                                                                                   \
        CERBLIB_DECL auto operator==(const Type &other) const noexcept -> bool                     \
        {                                                                                          \
            return value == other;                                                                 \
        }                                                                                          \
                                                                                                   \
        CERBLIB_DECL auto operator<=>(const Type &other) const noexcept                            \
        {                                                                                          \
            return value <=> other;                                                                \
        }                                                                                          \
                                                                                                   \
        CERBLIB_DECL auto operator<=>(const Name &) const noexcept = default;                      \
                                                                                                   \
        constexpr auto operator=(const Type &other) -> Name &                                      \
        {                                                                                          \
            value = other;                                                                         \
            return *this;                                                                          \
        }                                                                                          \
                                                                                                   \
        constexpr auto operator=(Type &&other) noexcept(std::is_nothrow_move_assignable_v<Type>)   \
            -> Name &                                                                              \
        {                                                                                          \
            value = std::move(other);                                                              \
            return *this;                                                                          \
        }                                                                                          \
                                                                                                   \
        Name() = default;                                                                          \
                                                                                                   \
        constexpr Name(const Type &value_) : value(value_)                                         \
        {}                                                                                         \
                                                                                                   \
        constexpr Name(Type &&value_) noexcept(std::is_nothrow_move_constructible_v<Type>)         \
          : value(std::move(value_))                                                               \
        {}                                                                                         \
                                                                                                   \
        static constexpr Type __VA_ARGS__;                                                         \
                                                                                                   \
    private:                                                                                       \
        Type value{};                                                                              \
    }

#endif /* CERBERUS_PROJECT_ENUM_HPP */

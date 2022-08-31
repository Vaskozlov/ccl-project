#ifndef CCL_PROJECT_ENUM_HPP
#define CCL_PROJECT_ENUM_HPP

#include <ccl/core/defines.hpp>

#define CCL_ENUM(Name, Type, ...)                                                                  \
    namespace detail::CCL_FORCE_EXPAND(Name, _)                                                    \
    {                                                                                              \
        struct IterationValues                                                                     \
        {                                                                                          \
            const Type __VA_ARGS__;                                                                \
        };                                                                                         \
                                                                                                   \
        static constexpr auto num_args = sizeof(IterationValues) / sizeof(Type);                   \
                                                                                                   \
        union IterationStruct {                                                                    \
            auto begin() const                                                                     \
            {                                                                                      \
                return unnamed_values.begin();                                                     \
            }                                                                                      \
                                                                                                   \
            auto end() const                                                                       \
            {                                                                                      \
                return unnamed_values.end();                                                       \
            }                                                                                      \
                                                                                                   \
            std::array<Type, num_args> unnamed_values;                                             \
            IterationValues iteration_values = IterationValues{};                                  \
        };                                                                                         \
    }                                                                                              \
                                                                                                   \
    struct Name                                                                                    \
    {                                                                                              \
        CCL_DECL operator Type() const                                                             \
        {                                                                                          \
            return value;                                                                          \
        }                                                                                          \
                                                                                                   \
        static auto contains(Name value) -> bool                                                   \
        {                                                                                          \
            return std::find(iteration_struct.begin(), iteration_struct.end(), value) !=           \
                   iteration_struct.end();                                                         \
        }                                                                                          \
                                                                                                   \
        CCL_DECL auto operator==(const Type &other) const noexcept -> bool                         \
        {                                                                                          \
            return value == other;                                                                 \
        }                                                                                          \
                                                                                                   \
        CCL_DECL auto operator<=>(const Type &other) const noexcept                                \
        {                                                                                          \
            return value <=> other;                                                                \
        }                                                                                          \
                                                                                                   \
        CCL_DECL auto operator<=>(const Name &) const noexcept = default;                          \
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
        static constexpr detail::CCL_FORCE_EXPAND(Name, _)::IterationStruct iteration_struct{};    \
                                                                                                   \
        Type value{};                                                                              \
    }

#endif /* CCL_PROJECT_ENUM_HPP */

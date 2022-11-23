#ifndef CCL_PROJECT_ENUM_HPP
#define CCL_PROJECT_ENUM_HPP

#include <ccl/core/defines.hpp>

#define CCL_PARENS ()

#define CCL_EXPAND(arg) CCL_EXPAND1(CCL_EXPAND1(CCL_EXPAND1(CCL_EXPAND1(arg))))
#define CCL_EXPAND1(arg) CCL_EXPAND2(CCL_EXPAND2(CCL_EXPAND2(CCL_EXPAND2(arg))))
#define CCL_EXPAND2(arg) CCL_EXPAND3(CCL_EXPAND3(CCL_EXPAND3(CCL_EXPAND3(arg))))
#define CCL_EXPAND3(arg) arg

#define CCL_FOR_EACH(macro, ...) __VA_OPT__(CCL_EXPAND(CCL_FOR_EACH_HELPER(macro, __VA_ARGS__)))
#define CCL_FOR_EACH_HELPER(macro, a1, ...)                                                        \
    macro(a1) __VA_OPT__(CCL_FOR_EACH_AGAIN CCL_PARENS(macro, __VA_ARGS__))
#define CCL_FOR_EACH_AGAIN() CCL_FOR_EACH_HELPER

#define CCL_ENUM_CASE(name)                                                                        \
case name:                                                                                         \
    return #name;

#define CCL_DECLARATION(name) static constexpr size_t name = __COUNTER__ - counter_begin - 1;

#define CCL_ENUM(Name, Type, ...)                                                                  \
    struct CCL_TRIVIAL_ABI Name                                                                    \
    {                                                                                              \
    private:                                                                                       \
        static constexpr auto counter_begin = __COUNTER__;                                         \
                                                                                                   \
    public:                                                                                        \
        CCL_DECL operator Type() const                                                             \
        {                                                                                          \
            return value;                                                                          \
        }                                                                                          \
                                                                                                   \
        CCL_DECL static auto contains(Name value) -> bool                                          \
        {                                                                                          \
            return value.value < (counter_end - counter_begin);                                    \
        }                                                                                          \
                                                                                                   \
        constexpr static auto toString(Type value) -> std::string_view                             \
        {                                                                                          \
            switch (value) {                                                                       \
                CCL_FOR_EACH(CCL_ENUM_CASE, __VA_ARGS__)                                           \
            default:                                                                               \
                return "unknown";                                                                  \
            }                                                                                      \
        }                                                                                          \
                                                                                                   \
        Name() = default;                                                                          \
                                                                                                   \
        constexpr Name(const Type &value_)                                                         \
          : value{value_}                                                                          \
        {}                                                                                         \
                                                                                                   \
        constexpr Name(Type &&value_) noexcept(std::is_nothrow_move_constructible_v<Type>)         \
          : value(std::move(value_))                                                               \
        {}                                                                                         \
                                                                                                   \
        CCL_FOR_EACH(CCL_DECLARATION, __VA_ARGS__)                                                 \
                                                                                                   \
    private:                                                                                       \
        static constexpr auto counter_end = __COUNTER__ - 1;                                       \
                                                                                                   \
        Type value{};                                                                              \
    }

#define CCL_PREDEFINED_ENUM(Name, Type, ...)                                                       \
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
        CCL_DECL auto isBaseOf(Name base) const noexcept -> bool                                   \
        {                                                                                          \
            return (value & base.value) == base.value;                                             \
        }                                                                                          \
                                                                                                   \
        CCL_DECL static auto isBaseOf(Name base, Name derived) noexcept -> bool                    \
        {                                                                                          \
            return (derived.value & base.value) == base.value;                                     \
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
        constexpr Name(const Type &value_)                                                         \
          : value(value_)                                                                          \
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

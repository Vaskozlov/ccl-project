#ifndef CCL_PROJECT_TYPES_HPP
#define CCL_PROJECT_TYPES_HPP

#include <bitset>
#include <ccl/core/pair.hpp>
#include <cinttypes>
#include <cmath>
#include <initializer_list>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ccl
{
    using i8 = std::int8_t;
    using i16 = std::int16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;

    using u8 = std::uint8_t;
    using u16 = std::uint16_t;
    using u32 = std::uint32_t;
    using u64 = std::uint64_t;

    using Id = std::size_t;
    using ssize_t = std::intmax_t;

    using f32 = std::float_t;
    using f64 = std::double_t;

    template<typename T>
    using InitializerList = const std::initializer_list<T> &;

    template<typename T, size_t Size>
    using CArray = T[Size];

    template<typename T, typename... Ts>
    constexpr auto makeUnique(Ts &&...args) -> std::unique_ptr<T>
        requires std::constructible_from<T, Ts...>
    {
        return std::make_unique<T>(std::forward<Ts>(args)...);
    }

    template<typename Target, typename Constructed, typename... Ts>
    constexpr auto makeUnique(Ts &&...args) -> std::unique_ptr<Target>
        requires std::derived_from<Constructed, Target> &&
                 std::constructible_from<Constructed, Ts...>
    {
        return std::unique_ptr<Target>{as<Target *>(new Constructed(std::forward<Ts>(args)...))};
    }

    template<typename T, typename... Ts>
    constexpr auto makeShared(Ts &&...args) -> std::shared_ptr<T>
        requires std::constructible_from<T, Ts...>
    {
        return std::make_shared<T>(std::forward<Ts>(args)...);
    }

    template<typename Target, typename Constructed, typename... Ts>
    constexpr auto makeShared(Ts &&...args) -> std::shared_ptr<Target>
        requires std::derived_from<Constructed, Target> &&
                 std::constructible_from<Constructed, Ts...>
    {
        return std::shared_ptr<Target>{as<Target *>(new Constructed(std::forward<Ts>(args)...))};
    }

    namespace integral_literals
    {
        constexpr auto operator"" _B(unsigned long long value) -> std::byte
        {
            return as<std::byte>(value);
        }
    }// namespace integral_literals
}// namespace ccl

#endif /* CCL_PROJECT_TYPES_HPP */

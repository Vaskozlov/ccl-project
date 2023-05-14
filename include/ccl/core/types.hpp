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
    using Optional = std::optional<T>;

    template<typename T, typename Alloc = std::allocator<T>>
    using Vector = std::vector<T, Alloc>;

    template<size_t N>
    using SmallBitset = std::bitset<N>;

    template<typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<T>>
    using Set = std::set<T, Compare, Alloc>;

    template<
        typename Key, typename Value, typename Compare = std::less<Key>,
        typename Alloc = std::allocator<std::pair<const Key, Value>>>
    using Map = std::map<Key, Value, Compare, Alloc>;

    template<
        typename T, typename Hash = std::hash<T>, typename Pred = std::equal_to<T>,
        typename Alloc = std::allocator<T>>
    using UnorderedSet = std::unordered_set<T, Hash, Pred, Alloc>;

    template<
        typename Key, typename Value, typename Hash = std::hash<Key>,
        typename Pred = std::equal_to<Key>,
        typename Alloc = std::allocator<std::pair<const Key, Value>>>
    using UnorderedMap = std::unordered_map<Key, Value, Hash, Pred, Alloc>;

    template<typename T>
    using InitializerList = const std::initializer_list<T> &;

    template<typename T>
    using UniquePtr = std::unique_ptr<T>;

    template<typename T>
    using SharedPtr = std::shared_ptr<T>;

    template<typename T>
    using WeakPtr = std::weak_ptr<T>;

    template<typename T, size_t Size>
    using CArray = T[Size];

    template<typename T, typename... Ts>
    constexpr auto makeUnique(Ts &&...args) -> UniquePtr<T>
        requires std::constructible_from<T, Ts...>
    {
        return std::make_unique<T>(std::forward<Ts>(args)...);
    }

    template<typename Target, typename Constructed, typename... Ts>
    constexpr auto makeUnique(Ts &&...args) -> UniquePtr<Target>
        requires std::derived_from<Constructed, Target> &&
                 std::constructible_from<Constructed, Ts...>
    {
        return UniquePtr<Target>{as<Target *>(new Constructed(std::forward<Ts>(args)...))};
    }

    template<typename T, typename... Ts>
    constexpr auto makeShared(Ts &&...args) -> SharedPtr<T>
        requires std::constructible_from<T, Ts...>
    {
        return std::make_shared<T>(std::forward<Ts>(args)...);
    }

    template<typename Target, typename Constructed, typename... Ts>
    constexpr auto makeShared(Ts &&...args) -> SharedPtr<Target>
        requires std::derived_from<Constructed, Target> &&
                 std::constructible_from<Constructed, Ts...>
    {
        return SharedPtr<Target>{as<Target *>(new Constructed(std::forward<Ts>(args)...))};
    }

    namespace integral_literals
    {
        constexpr auto operator"" _U8(unsigned long long value) -> u8
        {
            return as<u8>(value);
        }

        constexpr auto operator"" _U16(unsigned long long value) -> u16
        {
            return as<u16>(value);
        }

        constexpr auto operator"" _U32(unsigned long long value) -> u32
        {
            return as<u32>(value);
        }

        constexpr auto operator"" _U64(unsigned long long value) -> u64
        {
            return as<u64>(value);
        }

        constexpr auto operator"" _ZU(unsigned long long value) -> size_t
        {
            return as<size_t>(value);
        }

        constexpr auto operator"" _B(unsigned long long value) -> std::byte
        {
            return as<std::byte>(value);
        }
    }// namespace integral_literals
}// namespace ccl

#endif /* CCL_PROJECT_TYPES_HPP */

#ifndef CCL_PROJECT_TYPES_HPP
#define CCL_PROJECT_TYPES_HPP

#include <bitset>
#include <boost/container/flat_map.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/container/map.hpp>
#include <boost/container/set.hpp>
#include <boost/container/small_vector.hpp>
#include <ccl/core/pair.hpp>
#include <cinttypes>
#include <initializer_list>
#include <memory>
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

    using ssize_t = intmax_t;

    template<typename T>
    using Vector = std::vector<T>;

    template<size_t N>
    using SmallBitset = std::bitset<N>;

    template<typename T>
    using UnorderedSet = std::unordered_set<T>;

    template<typename Key, typename Value>
    using UnorderedMap = std::unordered_map<Key, Value>;

#ifndef CCL_DEBUG
    template<typename T>
    using Set = boost::container::set<T>;

    template<typename T>
    using UnorderedSet = std::unordered_set<T>;

    template<typename Key, typename Value>
    using Map = boost::container::map<Key, Value>;

    template<typename Key, typename Value>
    using UnorderedMap = std::unordered_map<Key, Value>;

    template<typename T>
    using FlatSet = boost::container::flat_set<T>;

    template<typename Key, typename Value>
    using Flatmap = boost::container::flat_map<Key, Value>;

    template<typename T, size_t N>
    using SmallVector = boost::container::small_vector<T, N>;
#else
    template<typename T>
    using Set = std::set<T>;

    template<typename Key, typename Value>
    using Map = std::map<Key, Value>;

    template<typename T>
    using FlatSet = std::set<T>;

    template<typename Key, typename Value>
    using Flatmap = std::map<Key, Value>;

    template<typename T, size_t N>
    using SmallVector = std::vector<T>;
#endif

    template<typename T>
    using InitializerList = const std::initializer_list<T> &;

    template<typename T>
    using UniquePtr = std::unique_ptr<T>;

    template<typename T>
    using SharedPtr = std::shared_ptr<T>;

    template<typename T, typename... Ts>
    constexpr auto makeUnique(Ts &&...args) -> UniquePtr<T>
    {
        return std::make_unique<T>(std::forward<Ts>(args)...);
    }

    template<typename Target, typename Constructed, typename... Ts>
        requires std::derived_from<Constructed, Target>
    constexpr auto makeUnique(Ts &&...args) -> UniquePtr<Target>
    {
        return UniquePtr<Target>{ static_cast<Target *>(
            new Constructed(std::forward<Ts>(args)...)) };
    }

    template<typename T, typename... Ts>
    constexpr auto makeShared(Ts &&...args) -> SharedPtr<T>
    {
        return std::make_shared<T>(std::forward<Ts>(args)...);
    }

    template<typename Target, typename Constructed, typename... Ts>
        requires std::derived_from<Constructed, Target>
    constexpr auto makeShared(Ts &&...args) -> SharedPtr<Target>
    {
        return SharedPtr<Target>{ static_cast<Target *>(
            new Constructed(std::forward<Ts>(args)...)) };
    }

    namespace integral_literals
    {
        constexpr auto operator"" _U8(unsigned long long value) -> u8
        {
            return static_cast<u8>(value);
        }

        constexpr auto operator"" _U16(unsigned long long value) -> u16
        {
            return static_cast<u16>(value);
        }

        constexpr auto operator"" _U32(unsigned long long value) -> u32
        {
            return static_cast<u32>(value);
        }

        constexpr auto operator"" _U64(unsigned long long value) -> u64
        {
            return static_cast<u64>(value);
        }

        constexpr auto operator"" _ZU(unsigned long long value) -> size_t
        {
            return static_cast<size_t>(value);
        }
    }// namespace integral_literals
}// namespace ccl

#endif /* CCL_PROJECT_TYPES_HPP */

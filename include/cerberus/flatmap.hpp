#ifndef CERBERUS_PROJECT_FLATMAP_HPP
#define CERBERUS_PROJECT_FLATMAP_HPP

#include <algorithm>
#include <array>
#include <cerberus/cerberus.hpp>
#include <initializer_list>
#include <utility>

namespace cerb
{
    template<typename Key, typename Value, size_t Size>
    class Flatmap
    {
    public:
        using key_type = Key;
        using mapped_type = Value;
        using value_type = std::pair<Key, Value>;
        using storage_t = std::array<value_type, Size>;
        using iterator = typename storage_t::iterator;
        using const_iterator = typename storage_t::const_iterator;

        CERBLIB_DECL auto size() const noexcept -> size_t
        {
            return occupied;
        }

        CERBLIB_DECL static auto capacity() noexcept -> size_t
        {
            return Size;
        }

        CERBLIB_DECL auto begin() noexcept -> iterator
        {
            return storage.begin();
        }

        CERBLIB_DECL auto end() noexcept -> iterator
        {
            return storage.begin() + occupied;
        }

        CERBLIB_DECL auto begin() const noexcept -> const_iterator
        {
            return storage.begin();
        }

        CERBLIB_DECL auto end() const noexcept -> const_iterator
        {
            return storage.begin() + occupied;
        }

        CERBLIB_DECL auto cbegin() const noexcept -> const_iterator
        {
            return storage.cbegin();
        }

        CERBLIB_DECL auto cend() const noexcept -> const_iterator
        {
            return storage.cbegin() + occupied;
        }

        constexpr auto insert(const Key &key, Value &&value) -> value_type &
        {
            return emplace(key, std::move(value));
        }

        constexpr auto insert(const Key &key, const Value &value) -> value_type &
        {
            return emplace(key, value);
        }

        constexpr auto insert(const value_type &value) -> value_type &
        {
            return emplace(value);
        }

        constexpr auto insert(value_type &&value) -> value_type &
        {
            return emplace(std::move(value));
        }

        template<typename... Ts>
        constexpr auto emplace(Ts &&...args) -> value_type &
        {
            if (occupied == capacity()) {
                throw OutOfRange("flatmap is full");
            }

            storage[occupied] = value_type(std::forward<Ts>(args)...);
            return storage[occupied++];
        }

        CERBLIB_DECL auto at(const Key &key) -> Value &
        {
            return staticAt(*this, key);
        }

        CERBLIB_DECL auto at(const Key &key) const -> const Value &
        {
            return staticAt(*this, key);
        }

        CERBLIB_DECL auto operator[](const Key &key) -> Value &
        {
            return at(key);
        }

        CERBLIB_DECL auto operator[](const Key &key) const -> const Value &
        {
            return at(key);
        }

        CERBLIB_DECL auto contains(const Key &key) const noexcept -> bool
        {
            return find(key) != end();
        }

        CERBLIB_DECL auto find(const Key &key) noexcept -> iterator
        {
            return staticFind(*this, key);
        }

        CERBLIB_DECL auto find(const Key &key) const noexcept -> const_iterator
        {
            return staticFind(*this, key);
        }

        Flatmap() = default;

        constexpr Flatmap(const std::initializer_list<value_type> &initial_data)
        {
            for (auto &value : initial_data) {
                insert(value);
            }
        }

    private:
        template<typename Self>
        CERBLIB_DECL static auto staticFind(Self &self, const Key &key) noexcept
            -> std::conditional_t<std::is_const_v<Self>, const_iterator, iterator>
        {
            return std::ranges::find_if(
                self, [key](const value_type &value) { return value.first == key; });
        }

        template<typename Self>
        CERBLIB_DECL static auto staticAt(Self &self, const Key &key)
            -> std::conditional_t<std::is_const_v<Self>, const Value &, Value &>
        {
            auto elem = self.find(key);

            if (elem == self.end()) {
                throw KeyNotFound("key not found");
            }

            return elem->second;
        }

        storage_t storage{};
        size_t occupied{};
    };
}// namespace cerb

#endif /* CERBERUS_PROJECT_FLATMAP_HPP */

#ifndef CCL_PROJECT_FLATMAP_HPP
#define CCL_PROJECT_FLATMAP_HPP

#include <ccl/ccl.hpp>
#include <utility>

namespace ccl
{
    template<typename Key, typename Value, size_t Size>
    class StaticFlatmap
    {
    public:
        using key_type = Key;
        using mapped_type = Value;
        using value_type = Pair<Key, Value>;
        using storage_t = std::array<value_type, Size>;
        using iterator = typename storage_t::iterator;
        using const_iterator = typename storage_t::const_iterator;

        CCL_DECL auto size() const noexcept -> size_t
        {
            return occupied;
        }

        CCL_DECL static auto capacity() noexcept -> size_t
        {
            return Size;
        }

        CCL_DECL auto begin() noexcept -> iterator
        {
            return storage.begin();
        }

        CCL_DECL auto end() noexcept -> iterator
        {
            return storage.begin() + occupied;
        }

        CCL_DECL auto begin() const noexcept -> const_iterator
        {
            return storage.begin();
        }

        CCL_DECL auto end() const noexcept -> const_iterator
        {
            return storage.begin() + occupied;
        }

        CCL_DECL auto cbegin() const noexcept -> const_iterator
        {
            return storage.cbegin();
        }

        CCL_DECL auto cend() const noexcept -> const_iterator
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
                throw std::out_of_range("flatmap is full");
            }

            storage[occupied] = value_type(std::forward<Ts>(args)...);
            return storage[occupied++];
        }

        CCL_DECL auto at(const Key &key) -> Value &
        {
            return staticAt(*this, key);
        }

        CCL_DECL auto at(const Key &key) const -> const Value &
        {
            return staticAt(*this, key);
        }

        CCL_DECL auto operator[](const Key &key) -> Value &
        {
            return at(key);
        }

        CCL_DECL auto operator[](const Key &key) const -> const Value &
        {
            return at(key);
        }

        CCL_DECL auto contains(const Key &key) const noexcept -> bool
        {
            return find(key) != end();
        }

        CCL_DECL auto find(const Key &key) noexcept -> iterator
        {
            return staticFind(*this, key);
        }

        CCL_DECL auto find(const Key &key) const noexcept -> const_iterator
        {
            return staticFind(*this, key);
        }

        StaticFlatmap() = default;

        constexpr StaticFlatmap(InitializerList<value_type> initial_data)
        {
            for (const auto &value : initial_data) {
                insert(value);
            }
        }

    private:
        template<typename Self>
        CCL_DECL static auto staticFind(Self &self, const Key &key) noexcept
        {
            return std::ranges::find_if(
                self, [&key](const value_type &value) { return value.first == key; });
        }

        template<typename Self>
        CCL_DECL static auto &staticAt(Self &self, const Key &key)
        {
            auto elem = self.find(key);

            if (elem == self.end()) {
                throw std::out_of_range("key not found");
            }

            return elem->second;
        }

        storage_t storage{};
        size_t occupied{};
    };
}// namespace ccl

#endif /* CCL_PROJECT_FLATMAP_HPP */

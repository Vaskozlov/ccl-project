#ifndef CCL_PROJECT_FLATMAP_HPP
#define CCL_PROJECT_FLATMAP_HPP

#include <ccl/ccl.hpp>
#include <ccl/iterator.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>
#include <utility>

namespace ccl
{
    template<typename Key, typename Value, size_t Size, typename Pred = std::equal_to<>>
    class StaticFlatmap : public AutoIterator<StaticFlatmap<Key, Value, Size, Pred>>
    {
    public:
        using key_type = Key;
        using mapped_type = Value;
        using value_type = Pair<Key, Value>;
        using storage_t = std::array<value_type, Size>;
        using iterator = typename storage_t::iterator;
        using const_iterator = typename storage_t::const_iterator;

    private:
        storage_t storage{};
        size_t occupied{};

    public:
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

        template<typename K, typename... Ts>
        constexpr auto tryEmplace(K &&key, Ts &&...args) -> Pair<iterator, bool>
            requires std::constructible_from<value_type, Key, Ts...>
        {
            if (occupied == capacity()) {
                throw std::out_of_range("flatmap is full");
            }

            auto result = find(key);

            if (result != end()) {
                return {result, false};
            }

            storage[occupied++] = value_type(std::forward<K>(key), std::forward<Ts>(args)...);
            return {result, true};
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
            return tryEmplace(key).first->second;
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
                if (occupied == capacity()) {
                    throw std::runtime_error{"StaticFlatmap capacity limit reached"};
                }

                storage[occupied++] = value;
            }
        }

    private:
        template<typename Self>
        CCL_DECL static auto staticFind(Self &self, const Key &key) noexcept -> auto
        {
            return std::find_if(std::begin(self), std::end(self), [&key](const value_type &value) {
                return Pred{}(key, value.first);
            });
        }

        template<typename Self>
        CCL_DECL static auto staticAt(Self &self, const Key &key) -> auto &
        {
            auto elem = self.find(key);

            if (elem == self.end()) {
                throw std::out_of_range("key not found");
            }

            return elem->second;
        }
    };
}// namespace ccl

#endif /* CCL_PROJECT_FLATMAP_HPP */

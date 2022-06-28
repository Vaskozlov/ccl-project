#ifndef CERBERUS_PROJECT_TYPED_BITSET_HPP
#define CERBERUS_PROJECT_TYPED_BITSET_HPP

#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include <cerberus/cerberus.hpp>

namespace cerb
{
    template<std::integral T>
    class TypedBitset
    {
    private:
        static constexpr bool is_small_bitset = sizeof(T) == sizeof(u8);
        static constexpr size_t small_bitset_size = 1ULL << (sizeof(T) * 8);

        using storage_t = std::conditional_t<
            is_small_bitset, std::bitset<small_bitset_size>, boost::dynamic_bitset<>>;

    public:
        CERBLIB_DECL auto capacity() const -> size_t
        {
            if constexpr (is_small_bitset) {
                return small_bitset_size;
            } else {
                return storage.capacity();
            }
        }

        constexpr auto set(T position, bool value) -> void
        {
            auto index = toIndex(position);
            resize(index + 1);

            storage.set(index, value);
        }

        constexpr auto set(T from, T to, bool value) -> void
        {
            auto begin_index = toIndex(from);
            auto end_index = toIndex(to);

            resize(end_index + 1);
            multiset(begin_index, end_index, value);
        }

        CERBLIB_DECL auto at(T position) const -> bool
        {
            auto index = toIndex(position);

            if (index >= storage.size()) {
                return false;
            }

            return storage.test(index);
        }

        CERBLIB_DECL auto operator[](T position) const -> bool
        {
            return at(position);
        }

        TypedBitset() = default;

        explicit TypedBitset(size_t size) requires(not is_small_bitset) : storage(size)
        {}

    private:
        constexpr auto multiset(size_t begin_index, size_t end_index, bool value) -> void
        {
            if constexpr (is_small_bitset) {
                for (size_t i = begin_index; i <= end_index; ++i) {
                    storage.set(i, value);
                }
            } else {
                storage.set(begin_index, end_index, value);
            }
        }

        constexpr auto resize(size_t size) -> void
        {
            if (size >= storage.size()) {
                if constexpr (is_small_bitset) {
                    throw std::logic_error("unable to resize small bitset");
                } else {
                    storage.resize(size);
                }
            }
        }

        CERBLIB_DECL static auto toIndex(T value) -> size_t
        {
            if constexpr (std::unsigned_integral<T>) {
                return static_cast<size_t>(value);
            } else {
                return static_cast<size_t>(
                    static_cast<ssize_t>(value) - std::numeric_limits<T>::min());
            }
        }

        storage_t storage{};
    };
}// namespace cerb

#endif /* CERBERUS_PROJECT_TYPED_BITSET_HPP */

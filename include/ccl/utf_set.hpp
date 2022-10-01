#ifndef CCL_PROJECT_UTF_SET_HPP
#define CCL_PROJECT_UTF_SET_HPP

#include <ccl/ccl.hpp>

namespace ccl
{
    class UtfSet
    {
    public:
        [[nodiscard]] auto empty() const noexcept -> bool
        {
            return small_storage.none() && storage.empty();
        }

        [[nodiscard]] auto at(char32_t chr) const noexcept -> bool
        {
            if (chr < small_storage_size) [[likely]] {
                return small_storage.test(chr);
            }

            return storage.contains(chr);
        }

        auto set(char32_t chr, bool value = true) -> void
        {
            if (chr < small_storage_size) [[likely]] {
                small_storage.set(chr, value);
            } else [[unlikely]] {
                if (value) {
                    storage.insert(chr);
                } else {
                    storage.erase(chr);
                }
            }
        }

        auto set(char32_t from, char32_t to, bool value = true) -> void
        {
            for (auto chr = from; chr <= to; ++chr) {
                set(chr, value);
            }
        }

        UtfSet() = default;

    private:
        static constexpr auto small_storage_size = 128ZU;

        SmallBitset<small_storage_size> small_storage{};
        FlatSet<char32_t> storage{};
    };
}// namespace ccl

#endif /* CCL_PROJECT_UTF_SET_HPP */

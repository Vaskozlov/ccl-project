#include <ccl/utf_set.hpp>

namespace ccl
{
    auto UtfSet::empty() const noexcept -> bool
    {
        return small_storage.none() && storage.empty();
    }

    auto UtfSet::at(char32_t chr) const noexcept -> bool
    {
        if (chr < small_storage_size) [[likely]] {
            return small_storage.test(chr);
        }

        return storage.contains(chr);
    }

    auto UtfSet::set(char32_t chr, bool value) -> void
    {
        if (chr < small_storage_size) [[likely]] {
            small_storage.set(chr, value);
        } else [[unlikely]] {
            if (value) [[likely]] {
                storage.insert(chr);
            } else [[unlikely]] {
                storage.erase(chr);
            }
        }
    }

    auto UtfSet::set(char32_t from, char32_t to, bool value) -> void
    {
        CCL_UNROLL_N(4)
        for (auto chr = from; chr <= to; ++chr) {
            set(chr, value);
        }
    }
}// namespace ccl

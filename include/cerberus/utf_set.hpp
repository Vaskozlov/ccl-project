#ifndef CERBERUS_PROJECT_UTF_SET_HPP
#define CERBERUS_PROJECT_UTF_SET_HPP

#include <bitset>
#include <unordered_set>

namespace cerb
{
    class UtfSet
    {
    public:
        auto empty() const -> bool;
        auto at(char32_t chr) const -> bool;

        auto set(char32_t chr, bool value = true) -> void;
        auto set(char32_t from, char32_t to, bool value = true) -> void;

        UtfSet() = default;

    private:
        static constexpr auto small_character_optimization_size = static_cast<size_t>(128);

        std::bitset<small_character_optimization_size> small_storage{};
        std::unordered_set<char32_t> storage{};
    };

    auto UtfSet::empty() const -> bool
    {
        return storage.empty() && small_storage.none();
    }

    auto UtfSet::at(char32_t chr) const -> bool
    {
        if (chr < small_character_optimization_size) {
            return small_storage.test(chr);
        }

        return storage.contains(chr);
    }

    auto UtfSet::set(char32_t chr, bool value) -> void
    {
        if (chr < small_character_optimization_size) {
            small_storage.set(chr, value);
        } else {
            if (value) {
                storage.insert(chr);
            } else {
                storage.erase(chr);
            }
        }
    }

    auto UtfSet::set(char32_t from, char32_t to, bool value) -> void
    {
        for (auto chr = from; chr <= to; ++chr) {
            set(chr, value);
        }
    }
}// namespace cerb

#endif /* CERBERUS_PROJECT_UTF_SET_HPP */

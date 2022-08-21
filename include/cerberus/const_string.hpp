#ifndef CERBERUS_PROJECT_CONST_STRING_HPP
#define CERBERUS_PROJECT_CONST_STRING_HPP

#include <algorithm>
#include <array>
#include <cerberus/cerberus.hpp>
#include <cerberus/string_view.hpp>

namespace cerb
{
    template<size_t N>
    struct ConstString
    {
        using value_type = char8_t;
        using storage_t = std::array<char8_t, N>;
        using const_iterator = typename storage_t::const_iterator;
        using const_reverse_iterator = typename storage_t::const_reverse_iterator;

        CERBLIB_DECL auto begin() const -> const_iterator
        {
            return string.begin();
        }

        CERBLIB_DECL auto end() const -> const_iterator
        {
            return string.begin() + size();
        }

        CERBLIB_DECL auto cbegin() const -> const_iterator
        {
            return begin();
        }

        CERBLIB_DECL auto cend() const -> const_iterator
        {
            return end();
        }

        CERBLIB_DECL auto rbegin() const -> const_reverse_iterator
        {
            return const_reverse_iterator{ end() };
        }

        CERBLIB_DECL auto rend() const -> const_reverse_iterator
        {
            return const_reverse_iterator{ begin() };
        }

        CERBLIB_DECL auto crbegin() const -> const_reverse_iterator
        {
            return rbegin();
        }

        CERBLIB_DECL auto crend() const -> const_reverse_iterator
        {
            return rend();
        }

        CERBLIB_DECL auto size() const -> size_t
        {
            return string.size() - 1;
        }

        CERBLIB_DECL auto empty() const -> bool
        {
            return size() == 0;
        }

        CERBLIB_DECL operator u8string_view() const  // NOLINT
        {
            return { string.data(), size() };
        }

        CERBLIB_DECL auto at(size_t index) const -> char8_t
        {
            return string.at(index);
        }

        CERBLIB_DECL auto operator[](size_t index) const -> char8_t
        {
            return string[index];
        }

        CERBLIB_DECL auto operator<=>(const ConstString &other) const
            -> std::weak_ordering = default;

        // NOLINTNEXTLINE
        consteval ConstString(const char8_t (&str)[N]) : string{ std::to_array(str) }
        {}

        // NOLINTNEXTLINE
        consteval ConstString(const std::array<char8_t, N> &str) : string{ str }
        {}

        storage_t string{};
    };
}// namespace cerb

#endif /* CERBERUS_PROJECT_CONST_STRING_HPP */

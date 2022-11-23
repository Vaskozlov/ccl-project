#ifndef CCL_PROJECT_CONST_STRING_HPP
#define CCL_PROJECT_CONST_STRING_HPP

#include <ccl/string_view.hpp>

namespace ccl
{
    template<size_t N>
    struct ConstString
    {
    public:
        using value_type = char;
        using storage_t = std::array<char, N>;
        using const_iterator = typename storage_t::const_iterator;
        using const_reverse_iterator = typename storage_t::const_reverse_iterator;

        storage_t string{};

        CCL_DECL auto begin() const noexcept -> const_iterator
        {
            return string.begin();
        }

        CCL_DECL auto end() const noexcept -> const_iterator
        {
            return string.begin() + size();
        }

        CCL_DECL auto cbegin() const noexcept -> const_iterator
        {
            return begin();
        }

        CCL_DECL auto cend() const noexcept -> const_iterator
        {
            return end();
        }

        CCL_DECL auto rbegin() const noexcept -> const_reverse_iterator
        {
            return const_reverse_iterator{end()};
        }

        CCL_DECL auto rend() const noexcept -> const_reverse_iterator
        {
            return const_reverse_iterator{begin()};
        }

        CCL_DECL auto crbegin() const noexcept -> const_reverse_iterator
        {
            return rbegin();
        }

        CCL_DECL auto crend() const noexcept -> const_reverse_iterator
        {
            return rend();
        }

        CCL_DECL auto size() const noexcept -> size_t
        {
            return string.size() - 1;
        }

        CCL_DECL auto empty() const noexcept -> bool
        {
            return size() == 0;
        }

        CCL_DECL operator string_view() const noexcept// NOLINT implicit conversion
        {
            return {string.data(), size()};
        }

        CCL_DECL auto at(size_t index) const -> char
        {
            return string.at(index);
        }

        CCL_DECL auto operator[](size_t index) const noexcept -> char
        {
            return string[index];
        }

        CCL_DECL auto operator<=>(const ConstString &other) const noexcept
            -> std::weak_ordering = default;

        // NOLINTNEXTLINE
        consteval ConstString(const value_type (&str)[N])
          : string{std::to_array(str)}
        {}

        // NOLINTNEXTLINE
        consteval ConstString(const std::array<value_type, N> &str)
          : string{str}
        {}
    };
}// namespace ccl

#endif /* CCL_PROJECT_CONST_STRING_HPP */

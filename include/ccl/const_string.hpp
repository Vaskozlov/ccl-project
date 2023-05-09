#ifndef CCL_PROJECT_CONST_STRING_HPP
#define CCL_PROJECT_CONST_STRING_HPP

#include <ccl/string_view.hpp>

namespace ccl
{
    template<size_t Size>
    class ConstString : public AutoIterator<ConstString<Size>>
    {
    public:
        using value_type = char;
        using storage_t = std::array<char, Size>;
        using const_iterator = typename storage_t::const_iterator;
        using const_reverse_iterator = typename storage_t::const_reverse_iterator;

        storage_t string{};

        // NOLINTNEXTLINE
        [[nodiscard]] consteval ConstString(const CArray<value_type, Size> &str)
          : string{std::to_array(str)}
        {}

        // NOLINTNEXTLINE
        [[nodiscard]] consteval ConstString(const std::array<value_type, Size> &str)
          : string{str}
        {}

        CCL_DECL auto begin() const noexcept -> const_iterator
        {
            return string.begin();
        }

        CCL_DECL auto end() const noexcept -> const_iterator
        {
            return string.begin() + size();
        }

        CCL_DECL auto size() const noexcept -> size_t
        {
            return string.size() - 1;
        }

        CCL_DECL auto empty() const noexcept -> bool
        {
            return 0 == size();
        }

        CCL_DECL operator ccl::string_view() const noexcept// NOLINT implicit conversion
        {
            return {string.data(), size()};
        }

        CCL_DECL operator std::string_view() const noexcept// NOLINT implicit conversion
        {
            return {string.begin(), string.begin() + size()};
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
    };
}// namespace ccl

#endif /* CCL_PROJECT_CONST_STRING_HPP */

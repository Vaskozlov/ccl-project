#ifndef CCL_PROJECT_STRING_VIEW_HPP
#define CCL_PROJECT_STRING_VIEW_HPP

#include <ccl/ccl.hpp>
#include <numeric>
#include <optional>
#include <stdexcept>

namespace ccl
{
    template<typename T>
    constexpr auto strlen(T &&str) noexcept -> size_t
    {
        return std::basic_string_view{std::forward<T>(str)}.size();
    }

    template<CharacterLiteral CharT>
    class BasicStringView;

    using string_view = BasicStringView<char>;
    using u8string_view = BasicStringView<char8_t>;
    using u16string_view = BasicStringView<char16_t>;
    using u32string_view = BasicStringView<char16_t>;
    using wstring_view = BasicStringView<wchar_t>;

    template<typename T, typename CharT>
    concept StringLike = std::is_same_v<T, BasicStringView<CharT>> ||
                         std::is_same_v<T, std::basic_string_view<CharT>> ||
                         std::is_same_v<T, std::basic_string<CharT>>;

    template<CharacterLiteral CharT>
    class BasicStringView
    {
    public:
        using pointer = const CharT *;
        using iterator = pointer;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;

        static constexpr auto npos = std::string_view::npos;

    private:
        pointer string{nullptr};
        size_t length{0};

    public:
        BasicStringView() noexcept = default;

        template<size_t N>
        constexpr explicit BasicStringView(const std::array<CharT, N> &array_) noexcept
          : string{array_.data()}, length{array_.size()}
        {}

        constexpr BasicStringView(pointer string_, size_t length_) noexcept
          : string{string_}, length{length_}
        {}

        constexpr BasicStringView(iterator first, iterator last) noexcept
          : string{first}, length{distance(first, last)}
        {}

        // NOLINTNEXTLINE
        constexpr BasicStringView(const CharacterArray auto &str) noexcept
          : string{str}, length{strlen(str)}
        {}

        // NOLINTNEXTLINE
        constexpr BasicStringView(const StringLike<CharT> auto &str) noexcept
          : string{std::data(str)}, length{std::size(str)}
        {}

        CCL_DECL auto size() const noexcept -> size_t
        {
            return length;
        }

        CCL_DECL auto empty() const noexcept -> bool
        {
            return size() == 0;
        }

        CCL_DECL auto data() const noexcept -> pointer
        {
            return begin();
        }

        CCL_DECL auto begin() const noexcept -> iterator
        {
            return string;
        }

        CCL_DECL auto end() const noexcept -> iterator
        {
            return string + length;
        }

        CCL_DECL auto cbegin() const noexcept -> iterator
        {
            return string;
        }

        CCL_DECL auto cend() const noexcept -> iterator
        {
            return string + length;
        }

        CCL_DECL auto rbegin() const noexcept -> reverse_iterator
        {
            return reverse_iterator{end()};
        }

        CCL_DECL auto rend() const noexcept -> reverse_iterator
        {
            return reverse_iterator{begin()};
        }

        CCL_DECL auto crbegin() const noexcept -> reverse_iterator
        {
            return reverse_iterator{end()};
        }

        CCL_DECL auto crend() const noexcept -> reverse_iterator
        {
            return reverse_iterator{begin()};
        }

        CCL_DECL auto substr(size_t first) const noexcept -> BasicStringView
        {
            first = first > size() ? size() : first;
            return {begin() + first, begin() + size()};
        }

        CCL_DECL auto substr(size_t first, size_t len) const noexcept -> BasicStringView
        {
            auto last = first + len;
            first = first > size() ? size() : first;
            last = last > size() ? size() : last;
            return {begin() + first, begin() + last};
        }

        CCL_UNSAFE_VERSION
        CCL_DECL auto find(CharT chr, size_t offset = 0) const noexcept -> size_t
        {
            if (offset >= length) {
                return npos;
            }

            auto elem = std::find(begin() + offset, end(), chr);
            return elem == end() ? npos : distance(begin(), elem);
        }

        CCL_SAFE_VERSION
        CCL_DECL auto find(CharT chr, size_t offset = 0) const noexcept -> Optional<size_t>
        {
            auto result = find<UNSAFE>(chr, offset);

            if (result == npos) {
                return std::nullopt;
            }

            return result;
        }

        CCL_UNSAFE_VERSION
        CCL_DECL auto find(CharT chr, iterator from) const noexcept -> size_t
        {
            if (from >= end()) {
                return npos;
            }

            auto elem = std::find(from, end(), chr);
            return elem == end() ? npos : distance(begin(), elem);
        }

        CCL_SAFE_VERSION
        CCL_DECL auto find(CharT chr, iterator from) const noexcept -> Optional<size_t>
        {
            auto result = find<UNSAFE>(chr, from);

            if (result == npos) {
                return std::nullopt;
            }

            return result;
        }

        CCL_UNSAFE_VERSION
        CCL_DECL auto openCloseFind(CharT open, CharT close) const noexcept -> size_t
        {
            auto passed_pairs = 0ZU;

            auto elem = std::ranges::find_if(*this, [&passed_pairs, open, close](CharT chr) {
                passed_pairs += (chr == open);
                passed_pairs -= (chr == close);
                return passed_pairs == 0;
            });

            if (elem == end()) {
                return npos;
            }

            return distance(begin(), elem);
        }

        CCL_SAFE_VERSION
        CCL_DECL auto openCloseFind(CharT open, CharT close) const noexcept -> Optional<size_t>
        {
            auto result = openCloseFind<UNSAFE>(open, close);

            if (result == npos) {
                return std::nullopt;
            }

            return result;
        }

        CCL_UNSAFE_VERSION
        CCL_DECL auto rfind(CharT chr, size_t offset = 0) const noexcept -> size_t
        {
            if (offset >= length) {
                return npos;
            }

            auto elem = std::find(rbegin() + as<long>(offset), rend(), chr);
            return elem == rend() ? npos : distance(elem, rend()) - 1;
        }

        CCL_SAFE_VERSION
        CCL_DECL auto rfind(CharT chr, size_t offset = 0) const noexcept -> Optional<size_t>
        {
            auto result = rfind<UNSAFE>(chr, offset);

            if (result == npos) {
                return std::nullopt;
            }

            return result;
        }

        CCL_UNSAFE_VERSION
        constexpr auto setLength(size_t new_length) -> void
        {
            length = new_length;
        }

        CCL_SAFE_VERSION
        constexpr auto setLength(size_t new_length) -> void
        {
            if (new_length > length) {
                throw std::invalid_argument{"New length is greater than the old one"};
            }

            setLength<UNSAFE>(new_length);
        }

        CCL_DECL auto startsWith(const StringLike<CharT> auto &str) const noexcept -> bool
        {
            return substr(0, str.size()).operator==(str);
        }

        CCL_DECL auto operator[](size_t index) const noexcept -> CharT
        {
            return string[index];
        }

        CCL_DECL auto at(size_t index) const -> CharT
        {
            if (index >= length) {
                throw std::out_of_range("index out of range");
            }

            return string[index];
        }

        CCL_DECL explicit operator std::basic_string<CharT>() const
        {
            return {string, length};
        }

        CCL_DECL explicit operator std::basic_string_view<CharT>() const noexcept
        {
            return {string, length};
        }

        CCL_DECL CCL_INLINE auto operator==(const CharT *other) const noexcept -> bool
        {
            return this->operator==(BasicStringView{other});
        }

        CCL_DECL CCL_INLINE auto operator==(const StringLike<CharT> auto &other) const noexcept
            -> bool
        {
            return std::ranges::equal(*this, other);
        }

        CCL_DECL CCL_INLINE auto operator<=>(const CharT *other) const noexcept
            -> std::weak_ordering
        {
            return this->operator<=>(BasicStringView{other});
        }

        CCL_DECL auto operator<=>(const StringLike<CharT> auto &other) const noexcept
            -> std::weak_ordering
        {
            auto min_size = std::min(size(), std::size(other));

            for (auto i = 0ZU; i != min_size; ++i) {
                if (this->operator[](i) != other[i]) {
                    return this->operator[](i) <=> other[i];
                }
            }

            return size() <=> std::size(other);
        }

    private:
        template<typename T>
        CCL_DECL static auto distance(T first, T last) noexcept -> size_t
        {
            return as<size_t>(std::distance(first, last));
        }
    };

    extern template class BasicStringView<char>;
    extern template class BasicStringView<char8_t>;
    extern template class BasicStringView<char16_t>;
    extern template class BasicStringView<char32_t>;
    extern template class BasicStringView<wchar_t>;

    namespace string_view_literals
    {
        consteval auto operator""_sv(const char *string, size_t length) -> BasicStringView<char>
        {
            return {string, length};
        }

        consteval auto operator""_sv(const char8_t *string, size_t length)
            -> BasicStringView<char8_t>
        {
            return {string, length};
        }

        consteval auto operator""_sv(const char16_t *string, size_t length)
            -> BasicStringView<char16_t>
        {
            return {string, length};
        }

        consteval auto operator""_sv(const char32_t *string, size_t length)
            -> BasicStringView<char32_t>
        {
            return {string, length};
        }

        consteval auto operator""_sv(const wchar_t *string, size_t length)
            -> BasicStringView<wchar_t>
        {
            return {string, length};
        }
    }// namespace string_view_literals
}// namespace ccl

template<>
struct fmt::formatter<ccl::string_view> : fmt::formatter<std::string_view>
{
    auto format(const ccl::string_view &str, format_context &ctx)
    {
        return formatter<std::string_view>::format(as<std::string_view>(str), ctx);
    }
};

#endif /* CCL_PROJECT_STRING_VIEW_HPP*/

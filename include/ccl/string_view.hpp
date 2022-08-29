#ifndef CCL_PROJECT_STRING_VIEW_HPP
#define CCL_PROJECT_STRING_VIEW_HPP

#include <ccl/ccl.hpp>
#include <numeric>
#include <optional>
#include <string_view>

namespace ccl
{
    template<CharacterArray T>
    constexpr auto strlen(const T &str) noexcept -> size_t
    {
        if constexpr (std::is_pointer_v<T>) {
            if (str == nullptr) {
                return 0;
            }
        }

        return std::basic_string_view{ str }.size();
    }

    template<CharacterLiteral CharT>
    struct BasicStringView;

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
    struct BasicStringView
    {
        using pointer = const CharT *;
        using iterator = pointer;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;

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
            return reverse_iterator{ end() };
        }

        CCL_DECL auto rend() const noexcept -> reverse_iterator
        {
            return reverse_iterator{ begin() };
        }

        CCL_DECL auto crbegin() const noexcept -> reverse_iterator
        {
            return reverse_iterator{ end() };
        }

        CCL_DECL auto crend() const noexcept -> reverse_iterator
        {
            return reverse_iterator{ begin() };
        }

        CCL_DECL auto substr(size_t first) const noexcept -> BasicStringView
        {
            first = first > size() ? size() : first;
            return { begin() + first, begin() + size() };
        }

        CCL_DECL auto substr(size_t first, size_t len) const noexcept -> BasicStringView
        {
            auto last = first + len;
            first = first > size() ? size() : first;
            last = last > size() ? size() : last;
            return { begin() + first, begin() + last };
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
        CCL_DECL auto find(CharT chr, size_t offset = 0) const noexcept -> std::optional<size_t>
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
        CCL_DECL auto find(CharT chr, iterator from) const noexcept -> std::optional<size_t>
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
            auto passed_pairs = static_cast<size_t>(0);

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
        CCL_DECL auto openCloseFind(CharT open, CharT close) const noexcept -> std::optional<size_t>
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

            auto elem = std::find(rbegin() + static_cast<long>(offset), rend(), chr);
            return elem == rend() ? npos : distance(elem, rend()) - 1;
        }

        CCL_SAFE_VERSION
        CCL_DECL auto rfind(CharT chr, size_t offset = 0) const noexcept -> std::optional<size_t>
        {
            auto result = rfind<UNSAFE>(chr, offset);

            if (result == npos) {
                return std::nullopt;
            }

            return result;
        }

        CCL_DECL auto startsWith(const BasicStringView &str) const noexcept -> bool
        {
            return substr(0, str.size()) == str;
        }

        CCL_DECL auto operator[](size_t index) const noexcept -> CharT
        {
            return string[index];
        }

        CCL_DECL auto at(size_t index) const -> CharT
        {
            if (index >= length) {
                throw OutOfRange("index out of range");
            }

            return string[index];
        }

        CCL_DECL explicit operator std::basic_string<CharT>() const
        {
            return { string, length };
        }

        CCL_DECL explicit operator std::basic_string_view<CharT>() const noexcept
        {
            return { string, length };
        }

        CCL_DECL auto operator==(const CharT *other) const noexcept -> bool
        {
            return this->operator==(BasicStringView{ other });
        }

        CCL_DECL auto operator==(const StringLike<CharT> auto &other) const noexcept -> bool
        {
            return std::ranges::equal(*this, other);
        }

        CCL_DECL auto operator<=>(const CharT *other) const noexcept -> bool
        {
            return this->operator<=>(BasicStringView{ other });
        }

        CCL_DECL auto operator<=>(const StringLike<CharT> auto &other) const noexcept
            -> std::weak_ordering
        {
            auto max_length = std::min(size(), std::size(other));

            for (size_t i = 0; i != max_length; ++i) {
                if (operator[](i) != other[i]) {
                    return operator[](i) <=> other[i];
                }
            }

            return size() <=> std::size(other);
        }

        BasicStringView() noexcept = default;

        template<size_t N>
        constexpr explicit BasicStringView(const std::array<CharT, N> &array_) noexcept
          : string{ array_.data() }, length{ array_.size() }
        {}

        constexpr BasicStringView(pointer string_, size_t length_) noexcept
          : string{ string_ }, length{ length_ }
        {}

        constexpr BasicStringView(iterator first, iterator last) noexcept
          : string{ first }, length{ distance(first, last) }
        {}

        // NOLINTNEXTLINE
        constexpr BasicStringView(const CharacterArray auto &str) noexcept
          : string{ str }, length{ strlen(str) }
        {}

        // NOLINTNEXTLINE
        constexpr BasicStringView(const StringLike<CharT> auto &str) noexcept
          : string{ std::data(str) }, length{ std::size(str) }
        {}

        static constexpr auto npos = std::string_view::npos;

    private:
        template<typename T>
        CCL_DECL static auto distance(T first, T last) noexcept -> size_t
        {
            return static_cast<size_t>(std::distance(first, last));
        }

        pointer string{ nullptr };
        size_t length{ 0 };
    };

    namespace string_view_literals
    {
        constexpr auto operator""_sv(const char *string, size_t length) -> BasicStringView<char>
        {
            return { string, length };
        }

        constexpr auto operator""_sv(const char8_t *string, size_t length)
            -> BasicStringView<char8_t>
        {
            return { string, length };
        }

        constexpr auto operator""_sv(const char16_t *string, size_t length)
            -> BasicStringView<char16_t>
        {
            return { string, length };
        }

        constexpr auto operator""_sv(const char32_t *string, size_t length)
            -> BasicStringView<char32_t>
        {
            return { string, length };
        }

        constexpr auto operator""_sv(const wchar_t *string, size_t length)
            -> BasicStringView<wchar_t>
        {
            return { string, length };
        }
    }// namespace string_view_literals
}// namespace ccl

template<>
class fmt::formatter<ccl::string_view>
{
public:
    constexpr static auto parse(format_parse_context &ctx)
    {
        return ctx.begin();
    }

    template<typename FmtContext>
    constexpr auto format(const ccl::string_view &str, FmtContext &ctx) const
    {
        return format_to(ctx.out(), "{}", static_cast<std::string_view>(str));
    }
};

#endif /* CCL_PROJECT_STRING_VIEW_HPP*/

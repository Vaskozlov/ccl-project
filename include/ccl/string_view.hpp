#ifndef CCL_PROJECT_STRING_VIEW_HPP
#define CCL_PROJECT_STRING_VIEW_HPP

#include <ccl/ccl.hpp>
#include <ccl/iterator.hpp>
#include <numeric>

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
    concept StringLike = IsSameToAny<
        std::remove_cvref_t<T>, BasicStringView<CharT>, std::basic_string_view<CharT>,
        std::basic_string<CharT>>;

    template<CharacterLiteral CharT>
    class BasicStringView : public AutoIterator<BasicStringView<CharT>>
    {
    public:
        using pointer = const CharT *;
        using iterator = pointer;
        using value_type = CharT;
        using const_iterator = iterator;
        using reverse_iterator = std::reverse_iterator<iterator>;

        static constexpr auto npos = std::string_view::npos;

    private:
        pointer string{nullptr};
        size_t length{0};

    public:
        BasicStringView() noexcept = default;

        template<size_t N>
        constexpr explicit BasicStringView(const std::array<CharT, N> &char_array) noexcept
          : string{char_array.data()}
          , length{char_array.size()}
        {}

        constexpr BasicStringView(pointer char_pointer, size_t string_length) noexcept
          : string{char_pointer}
          , length{string_length}
        {}

        constexpr BasicStringView(iterator first, iterator last) noexcept
          : string{first}
          , length{distance(first, last)}
        {}

        // NOLINTNEXTLINE
        constexpr BasicStringView(const CharacterArray auto &str) noexcept
          : string{str}
          , length{strlen(str)}
        {}

        // NOLINTNEXTLINE
        constexpr BasicStringView(std::string_view str) noexcept
          : string{str.data()}
          , length{str.size()}
        {}

        // NOLINTNEXTLINE
        constexpr BasicStringView(const std::string &str) noexcept
          : string{str.c_str()}
          , length{str.size()}
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

        CCL_DECL auto substr(size_t first) const noexcept -> BasicStringView
        {
            first = std::min(size(), first);

            return {begin() + first, end()};
        }

        CCL_DECL auto substr(size_t first, size_t len) const noexcept -> BasicStringView
        {
            auto last = first + len;

            last = std::min(size(), last);
            first = std::min(size(), first);

            return {begin() + first, begin() + last};
        }

        CCL_DECL auto removeLastCharacter() const noexcept -> BasicStringView
        {
            if (empty()) {
                return *this;
            }

            auto truncated = *this;
            --truncated.length;

            return truncated;
        }

        CCL_UNSAFE_VERSION
        CCL_DECL auto find(CharT chr, size_t offset = 0) const noexcept -> size_t
        {
            if (offset >= length) {
                return npos;
            }

            const auto it_to_elem = std::find(begin() + offset, end(), chr);
            return it_to_elem == end() ? npos : distance(begin(), it_to_elem);
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

            const auto it_to_elem = std::find(from, end(), chr);
            return it_to_elem == end() ? npos : distance(begin(), it_to_elem);
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

        CCL_DECL auto contains(CharT chr) const noexcept -> bool
        {
            return find<SAFE>(chr).has_value();
        }

        /**
         * @brief returns index of the element, which closes range opened with starter
         * @param starter character, that starts range
         * @param ender character, that ends range
         * @return index on success or npos on failure
         */
        CCL_UNSAFE_VERSION
        CCL_DECL auto openCloseFind(CharT starter, CharT ender) const noexcept -> size_t
        {
            auto passed_pairs = as<size_t>(0);

            const auto it_to_elem =
                std::find_if(begin(), end(), [&passed_pairs, starter, ender](CharT chr) {
                    passed_pairs += (chr == starter);
                    passed_pairs -= (chr == ender);
                    return 0 == passed_pairs;
                });

            if (it_to_elem == end()) {
                return npos;
            }

            return distance(begin(), it_to_elem);
        }

        /**
         * @brief returns index of the element, which closes range opened with starter
         * @param starter character, that starts range
         * @param ender character, that ends range
         * @return index on success or std::nullopt failure
         */
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

            const auto it_to_elem = std::find(this->rbegin() + as<long>(offset), this->rend(), chr);
            return it_to_elem == this->rend() ? npos : distance(it_to_elem, this->rend()) - 1;
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

        CCL_DECL auto leftStrip(BasicStringView characters_to_strip) const noexcept
            -> BasicStringView
        {
            auto stripped_string = *this;
            auto has_characters_to_strip = [&stripped_string, &characters_to_strip]() {
                const auto first_character = *stripped_string.begin();
                return characters_to_strip.contains(first_character);
            };

            while (!stripped_string.empty() && has_characters_to_strip()) {
                --stripped_string.length;
                ++stripped_string.string;
            }

            return stripped_string;
        }

        CCL_DECL auto rightStrip(BasicStringView characters_to_strip) const noexcept
            -> BasicStringView
        {
            auto stripped_string = *this;
            auto has_characters_to_strip = [&stripped_string, &characters_to_strip]() {
                const auto last_character = *(stripped_string.end() - 1);
                return characters_to_strip.contains(last_character);
            };

            while (!stripped_string.empty() && has_characters_to_strip()) {
                --stripped_string.length;
            }

            return stripped_string;
        }

        CCL_DECL auto strip(BasicStringView characters_to_strip) const noexcept -> BasicStringView
        {
            const auto left_stripped = leftStrip(characters_to_strip);
            return left_stripped.rightStrip(characters_to_strip);
        }

        CCL_UNSAFE_VERSION
        CCL_DECL auto changeLength(size_t new_length) const noexcept -> BasicStringView
        {
            auto new_string = *this;
            new_string.length = new_length;
            return new_string;
        }

        CCL_SAFE_VERSION
        CCL_DECL auto changeLength(size_t new_length) const -> BasicStringView
        {
            if (new_length > length) {
                throw std::invalid_argument{"New length is greater than the old one"};
            }

            return changeLength<UNSAFE>(new_length);
        }

        CCL_DECL auto startsWith(const StringLike<CharT> auto &str) const noexcept -> bool
        {
            return substr(0, str.size()).operator==(str);
        }

        CCL_DECL auto operator[](size_t index) const -> CharT
        {
            return at(index);
        }

        CCL_DECL auto at(size_t index) const -> CharT
        {
            if (index >= length) {
                throw std::out_of_range("index out of range");
            }

            return string[index];
        }

        template<std::constructible_from<const CharT *, size_t> T>
        CCL_DECL explicit operator T() const
        {
            return {string, length};
        }

        CCL_DECL auto operator==(BasicStringView other) const noexcept -> bool
        {
            return this->operator==(as<std::string_view>(other));
        }

        CCL_DECL auto operator==(const StringLike<CharT> auto &other) const noexcept -> bool
        {
            return std::equal(begin(), end(), std::begin(other), std::end(other));
        }

        CCL_DECL auto operator<=>(BasicStringView other) const noexcept -> std::weak_ordering
        {
            return this->operator<=>(as<std::string_view>(other));
        }

        CCL_DECL auto operator<=>(const StringLike<CharT> auto &other) const noexcept
            -> std::weak_ordering
        {
            auto min_size = std::min(size(), std::size(other));

            for (auto i = as<size_t>(0); i != min_size; ++i) {
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
    auto format(const ccl::string_view &str, format_context &ctx) const -> decltype(ctx.out());
};

#endif /* CCL_PROJECT_STRING_VIEW_HPP*/

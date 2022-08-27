#ifndef CCL_PROJECT_TOKEN_HPP
#define CCL_PROJECT_TOKEN_HPP

#include <boost/container/small_vector.hpp>
#include <ccl/text/text_iterator.hpp>
#include <string>
#include <utility>
#include <vector>

namespace ccl::lex
{
    // NOLINTNEXTLINE
    CCL_ENUM(ReservedTokenType, u32, EOI = 0, BAD_TOKEN = 1);

    struct TokenAttributes
    {
        TokenAttributes() = default;

        explicit TokenAttributes(const text::TextIterator &text_iterator_)
          : tabs_and_spaces(text_iterator_.getTabsAndSpaces()),
            location(text_iterator_.getLocation())
        {}

        std::u32string tabs_and_spaces{};
        text::Location location{};
    };

    class Token
    {
    public:
        Token() = default;

        CCL_PERFECT_FORWARDING(T, std::u8string)
        Token(
            TokenAttributes &&attributes_, const u8string_view &repr_, size_t id_, T &&value_ = {})
          : attributes(attributes_), value{ std::forward<T>(value_) }, repr{ repr_ }, id{ id_ }
        {}

        CCL_PERFECT_FORWARDING(T, std::u8string)
        Token(
            TokenAttributes &&attributes_, typename u8string_view::iterator begin_, size_t id_,
            T &&value_ = {})
          : Token(
                std::move(attributes_), { begin_, static_cast<size_t>(0) }, id_,
                std::forward<T>(value_))
        {}

        CCL_PERFECT_FORWARDING(T, std::u8string)
        Token(const text::TextIterator &text_iterator_, size_t id_, T &&value_ = {})
          : attributes(text_iterator_), value{ std::forward<T>(value_) },
            repr{ text_iterator_.getRemaining() }, id{ id_ }
        {}

        [[nodiscard]] auto getId() const noexcept -> size_t
        {
            return id;
        }

        [[nodiscard]] explicit operator bool() const noexcept
        {
            return getId() != 0;
        }

        [[nodiscard]] auto getRepr() const noexcept -> u8string_view
        {
            return repr;
        }

        [[nodiscard]] auto getValue() const noexcept -> u8string_view
        {
            if (value.empty()) {
                return repr;
            }

            return value;
        }

        [[nodiscard]] auto getPrefixes() const noexcept -> const std::vector<u8string_view> &
        {
            return prefixes;
        }

        [[nodiscard]] auto getPostfixes() const noexcept -> const std::vector<u8string_view> &
        {
            return postfixes;
        }

        [[nodiscard]] auto getTabsAndSpaces() const noexcept -> const std::u32string &
        {
            return attributes.tabs_and_spaces;
        }

        auto setEnd(typename u8string_view::iterator end_) noexcept -> void
        {
            repr = { repr.begin(), end_ };
        }

        auto addPrefix(u8string_view prefix) -> void
        {
            prefixes.push_back(prefix);
        }

        auto addPostfix(u8string_view postfix) -> void
        {
            postfixes.push_back(postfix);
        }

    private:
        std::vector<u8string_view> prefixes{};
        std::vector<u8string_view> postfixes{};
        TokenAttributes attributes{};
        std::u8string value{};
        u8string_view repr{};
        size_t id{};
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_TOKEN_HPP */

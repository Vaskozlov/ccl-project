#ifndef CCL_PROJECT_TOKEN_HPP
#define CCL_PROJECT_TOKEN_HPP

#include <ccl/text/text_iterator.hpp>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace ccl::lex
{
    // NOLINTNEXTLINE
    CCL_ENUM(ReservedTokenType, size_t, EOI, BAD_TOKEN);

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

        CCL_PERFECT_FORWARDING(T, std::optional<std::string>)
        Token(
            TokenAttributes &&attributes_, const string_view &repr_, size_t id_,
            T &&value_ = std::nullopt)
          : value(std::forward<T>(value_)), attributes(attributes_), repr(repr_), id(id_)
        {}

        CCL_PERFECT_FORWARDING(T, std::optional<std::string>)
        Token(
            TokenAttributes &&attributes_, typename string_view::iterator begin_, size_t id_,
            T &&value_ = std::nullopt)
          : Token(std::move(attributes_), { begin_, 0 }, id_, std::forward<T>(value_))
        {}

        CCL_PERFECT_FORWARDING(T, std::optional<std::string>)
        Token(const text::TextIterator &text_iterator_, size_t id_, T &&value_ = std::nullopt)
          : value(std::forward<T>(value_)), attributes(text_iterator_),
            repr(text_iterator_.getRemaining()), id(id_)
        {}

        [[nodiscard]] auto getId() const noexcept -> size_t
        {
            return id;
        }

        [[nodiscard]] auto emptyRepr() const noexcept -> bool
        {
            return repr.empty();
        }

        [[nodiscard]] explicit operator bool() const noexcept
        {
            return getId() != 0;
        }

        [[nodiscard]] auto getLocation() const noexcept -> const text::Location &
        {
            return attributes.location;
        }

        [[nodiscard]] auto getLine() const noexcept -> size_t
        {
            return attributes.location.getLine();
        }

        [[nodiscard]] auto getColumn() const noexcept -> size_t
        {
            return attributes.location.getColumn();
        }

        [[nodiscard]] auto getRealColumn() const noexcept -> size_t
        {
            return attributes.location.getRealColumn();
        }

        [[nodiscard]] auto getFilename() const noexcept -> const string_view &
        {
            return attributes.location.getFilename();
        }

        [[nodiscard]] auto getRepr() const noexcept -> string_view
        {
            return repr;
        }

        [[nodiscard]] auto getValue() const noexcept -> string_view
        {
            if (not value.has_value()) {
                return repr;
            }

            return *value;
        }

        [[nodiscard]] auto getPrefixes() noexcept -> const std::vector<string_view> &
        {
            if (not prefixes.has_value()) {
                prefixes = std::vector<string_view>{};
            }

            // NOLINT access checked above
            return *prefixes;
        }

        [[nodiscard]] auto getPostfixes() noexcept -> const std::vector<string_view> &
        {
            if (not postfixes.has_value()) {
                postfixes = std::vector<string_view>{};
            }

            return *postfixes;
        }

        [[nodiscard]] auto getTabsAndSpaces() const noexcept -> const std::u32string &
        {
            return attributes.tabs_and_spaces;
        }

        auto setEnd(typename string_view::iterator end_) noexcept -> void
        {
            repr = { repr.begin(), end_ };
        }

        auto addPrefix(string_view prefix) -> void
        {
            if (not prefixes.has_value()) {
                prefixes = std::vector<string_view>{};
            }

            prefixes->push_back(prefix);
        }

        auto addPostfix(string_view postfix) -> void
        {
            if (not postfixes.has_value()) {
                postfixes = std::vector<string_view>{};
            }

            postfixes->push_back(postfix);
        }

    private:
        std::optional<std::vector<string_view>> prefixes{ std::nullopt };
        std::optional<std::vector<string_view>> postfixes{ std::nullopt };
        std::optional<std::string> value{};
        TokenAttributes attributes{};
        string_view repr{};
        size_t id{};
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_TOKEN_HPP */

#ifndef CCL_PROJECT_TOKEN_HPP
#define CCL_PROJECT_TOKEN_HPP

#include <ccl/text/text_iterator.hpp>

namespace ccl::lex
{
    // NOLINTNEXTLINE
    enum struct ReservedTokenType : Id
    {
        EOI = 0,
        BAD_TOKEN = 1
    };

    namespace dot_item
    {
        class Container;
    }

    class TokenAttributes
    {
    public:
        std::string tabsAndSpaces{};
        text::Location location{};
        string_view workingLine{};

        TokenAttributes() = default;

        explicit TokenAttributes(const text::TextIterator &text_iterator);
    };

    class Token
    {
    private:
        friend class LexicalAnalyzer;
        friend class dot_item::Container;

        Vector<string_view> prefixes{};
        Vector<string_view> postfixes{};
        TokenAttributes attributes{};
        string_view repr{};
        Id id{};

    public:
        Token() = default;

        [[nodiscard]] explicit Token(Id token_id);

        [[nodiscard]] Token(
            TokenAttributes &&token_attributes, string_view token_repr, Id token_id);

        [[nodiscard]] Token(
            TokenAttributes &&token_attributes, typename string_view::iterator text_begin,
            Id token_id);

        [[nodiscard]] Token(const text::TextIterator &text_iterator, Id token_id);

        [[nodiscard]] CCL_INLINE auto getId() const noexcept -> size_t
        {
            return id;
        }

        [[nodiscard]] CCL_INLINE auto emptyRepr() const noexcept -> bool
        {
            return repr.empty();
        }

        [[nodiscard]] CCL_INLINE explicit operator bool() const noexcept
        {
            return getId() != 0;
        }

        [[nodiscard]] CCL_INLINE auto getReprSize() const noexcept -> size_t
        {
            return repr.size();
        }

        [[nodiscard]] CCL_INLINE auto getLocation() const noexcept -> const text::Location &
        {
            return attributes.location;
        }

        [[nodiscard]] CCL_INLINE auto getLine() const noexcept -> size_t
        {
            return attributes.location.getLine();
        }

        [[nodiscard]] CCL_INLINE auto getColumn() const noexcept -> size_t
        {
            return attributes.location.getColumn();
        }

        [[nodiscard]] CCL_INLINE auto getRealColumn() const noexcept -> size_t
        {
            return attributes.location.getRealColumn();
        }

        [[nodiscard]] CCL_INLINE auto getFilename() const noexcept -> string_view
        {
            return attributes.location.getFilename();
        }

        [[nodiscard]] CCL_INLINE auto getRepr() const noexcept -> string_view
        {
            return repr;
        }

        [[nodiscard]] CCL_INLINE auto getPrefixes() const noexcept -> const Vector<string_view> &
        {
            return prefixes;
        }

        [[nodiscard]] CCL_INLINE auto getPostfixes() const noexcept -> const Vector<string_view> &
        {
            return postfixes;
        }

        [[nodiscard]] CCL_INLINE auto getInlineRepr() const noexcept -> string_view
        {
            return attributes.workingLine;
        }

        [[nodiscard]] CCL_INLINE auto getTabsAndSpaces() const noexcept -> std::string_view
        {
            return attributes.tabsAndSpaces;
        }

    private:
        auto clear(Id new_id) noexcept -> void;

        CCL_INLINE auto setReprLength(size_t length) noexcept -> void
        {
            repr.setLength<UNSAFE>(length);
        }

        CCL_INLINE auto addPrefix(string_view prefix) -> void
        {
            prefixes.push_back(prefix);
        }

        CCL_INLINE auto addPostfix(string_view postfix) -> void
        {
            postfixes.push_back(postfix);
        }

        CCL_INLINE auto setEnd(typename string_view::iterator end_of_repr) noexcept -> void
        {
            repr = {repr.begin(), end_of_repr};
        }

        auto finishInitialization(text::TextIterator &text_iterator, size_t totally_skipped)
            -> void;
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_TOKEN_HPP */

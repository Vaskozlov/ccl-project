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

    class TokenAttributes
    {
    public:
        std::string tabsAndSpaces{};
        text::Location location{};
        string_view workingLine{};

        TokenAttributes() = default;

        explicit TokenAttributes(const text::TextIterator &text_iterator)
          : tabsAndSpaces{text_iterator.getTabsAndSpaces()}
          , location{text_iterator.getLocation()}
          , workingLine{text_iterator.getWorkingLine()}
        {}
    };

    class Token
    {
        Vector<string_view> prefixes{};
        Vector<string_view> postfixes{};
        TokenAttributes attributes{};
        string_view repr{};
        Id id{};

    public:
        Token() = default;

        explicit Token(Id id_)
          : id{id_}
        {}

        Token(TokenAttributes &&token_attributes, const string_view &token_repr, Id token_id)
          : attributes{std::move(token_attributes)}
          , repr{token_repr}
          , id{token_id}
        {}

        Token(
            TokenAttributes &&token_attributes, typename string_view::iterator text_begin,
            Id token_id)
          : Token{std::move(token_attributes), {text_begin, as<size_t>(0)}, token_id}
        {}

        Token(const text::TextIterator &text_iterator, Id token_id)
          : attributes{text_iterator}
          , repr{text_iterator.getRemaining()}
          , id{token_id}
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

        [[nodiscard]] auto getReprSize() const noexcept -> size_t
        {
            return repr.size();
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

        [[nodiscard]] auto getPrefixes() const noexcept -> const Vector<string_view> &
        {
            return prefixes;
        }

        [[nodiscard]] auto getPostfixes() const noexcept -> const Vector<string_view> &
        {
            return postfixes;
        }

        [[nodiscard]] auto getWorkingLine() const noexcept -> const string_view &
        {
            return attributes.workingLine;
        }

        [[nodiscard]] auto getTabsAndSpaces() const noexcept -> const std::string &
        {
            return attributes.tabsAndSpaces;
        }

        auto setEnd(typename string_view::iterator end_) noexcept -> void
        {
            repr = {repr.begin(), end_};
        }

        auto finishInitialization(const text::TextIterator &text_iterator) -> void
        {
            repr = text_iterator.getRemaining();
            attributes = TokenAttributes{text_iterator};
        }

        auto setReprLength(size_t length) noexcept -> void
        {
            repr.setLength<UNSAFE>(length);
        }

        auto addPrefix(string_view prefix) -> void
        {
            prefixes.push_back(prefix);
        }

        auto addPostfix(string_view postfix) -> void
        {
            postfixes.push_back(postfix);
        }

        auto clear(Id new_id) noexcept -> void
        {
            id = new_id;
            prefixes.clear();
            postfixes.clear();
        }
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_TOKEN_HPP */

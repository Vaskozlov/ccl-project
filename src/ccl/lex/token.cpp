#include <ccl/lex/token.hpp>

namespace ccl::lex
{
    TokenAttributes::TokenAttributes(const text::TextIterator &text_iterator)
      : tabsAndSpaces{text_iterator.getTabsAndSpaces()}
      , location{text_iterator.getLocation()}
      , workingLine{text_iterator.getWorkingLine()}
    {}

    Token::Token(Id token_id)
      : id{token_id}
    {}

    Token::Token(TokenAttributes &&token_attributes, string_view token_repr, Id token_id)
      : attributes{std::move(token_attributes)}
      , repr{token_repr}
      , id{token_id}
    {}

    Token::Token(
        TokenAttributes &&token_attributes, typename string_view::iterator text_begin, Id token_id)
      : Token{std::move(token_attributes), {text_begin, as<size_t>(0)}, token_id}
    {}

    Token::Token(const text::TextIterator &text_iterator, Id token_id)
      : attributes{text_iterator}
      , repr{text_iterator.getRemaining()}
      , id{token_id}
    {}

    auto Token::clear(Id new_id) noexcept -> void
    {
        id = new_id;
        prefixes.clear();
        postfixes.clear();
    }

    auto Token::finishInitialization(text::TextIterator &text_iterator, size_t totally_skipped)
        -> void
    {
        repr = text_iterator.getRemaining();

        text_iterator.skip(1);
        attributes = TokenAttributes{text_iterator};

        setReprLength(totally_skipped);
        text_iterator.skip(totally_skipped - 1);
    }
}// namespace ccl::lex
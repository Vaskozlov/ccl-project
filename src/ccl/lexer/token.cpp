#include <ccl/lexer/token.hpp>

namespace ccl::lexer
{
    class TextIteratorWithSkippedChractersAccumulator
      : public text::CrtpBasicTextIterator<TextIteratorWithSkippedChractersAccumulator>
    {
    private:
        size_t skippedCharacters{};
        size_t skippedBytes{};

    public:
        using CrtpBasicTextIterator<
            TextIteratorWithSkippedChractersAccumulator>::CrtpBasicTextIterator;

        auto clearAccumulator() noexcept -> void
        {
            skippedBytes = 0;
            skippedCharacters = 0;
        }

        [[nodiscard]] constexpr auto getSkippedCharacters() const noexcept -> size_t
        {
            return skippedCharacters;
        }

        [[nodiscard]] constexpr auto getSkippedBytes() const noexcept -> size_t
        {
            return skippedBytes;
        }

        constexpr auto onMove(char /* chr */) noexcept -> void
        {
            ++skippedBytes;
        }

        constexpr auto onCharacter(char32_t /* chr */) noexcept -> void
        {
            ++skippedCharacters;
        }

        [[noreturn]] CCL_INLINE static auto utfError(char /* chr */) -> void
        {
            throw std::logic_error{"unable to decode utf8 symbol"};
        }
    };

    TokenEnvironment::TokenEnvironment(const text::TextIterator &text_iterator)
      : tabsAndSpaces{text_iterator.getTabsAndSpaces()}
      , location{text_iterator.getLocation()}
      , workingLine{text_iterator.getWorkingLine()}
    {}

    Token::Token(Id token_id)
      : id{token_id}
    {}

    Token::Token(TokenEnvironment &&token_environment, isl::string_view token_repr, Id token_id)
      : environment{std::move(token_environment)}
      , repr{token_repr}
      , id{token_id}
    {}

    Token::Token(
        TokenEnvironment &&token_environment, typename isl::string_view::iterator text_begin,
        Id token_id)
      : Token{std::move(token_environment), {text_begin, isl::as<size_t>(0)}, token_id}
    {}

    Token::Token(const text::TextIterator &text_iterator, Id token_id)
      : environment{text_iterator}
      , repr{text_iterator.getRemaining()}
      , id{token_id}
    {}

    auto Token::clear(Id new_id) noexcept -> void
    {
        id = new_id;
        prefixes.clear();
        postfixes.clear();
    }

    auto Token::cut(size_t first, size_t length) const -> Token
    {
        auto new_token = *this;
        new_token.clear(isl::as<size_t>(ReservedTokenType::CUT));

        auto text_iterator = TextIteratorWithSkippedChractersAccumulator(repr);
        text_iterator.skip(first);

        new_token.repr = text_iterator.getRemaining();
        new_token.environment.location = text::Location{
            getFilename(), getLine(), getColumn() + text_iterator.getSkippedCharacters(),
            getRealColumn() + text_iterator.getSkippedBytes()};
        new_token.environment.tabsAndSpaces.clear();

        text_iterator.clearAccumulator();
        text_iterator.skip(length);

        new_token.setEnd(new_token.repr.begin() + text_iterator.getSkippedBytes());

        return new_token;
    }

    auto Token::finishInitialization(text::TextIterator &text_iterator, size_t totally_skipped)
        -> void
    {
        text_iterator.skip(1);
        repr = text_iterator.getRemainingWithCurrent();
        environment = TokenEnvironment{text_iterator};

        setReprLength(totally_skipped);
        text_iterator.skip(totally_skipped - 1);
    }
}// namespace ccl::lexer
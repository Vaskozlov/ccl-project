#include <ccl/lexer/token.hpp>

namespace ccl::lexer
{
    CCL_INLINE static auto isTabOrSpace(char chr) noexcept -> bool
    {
        return '\t' == chr || ' ' == chr;
    }

    class TextIteratorWithSkippedCharactersAccumulator
      : public text::CrtpBasicTextIterator<TextIteratorWithSkippedCharactersAccumulator>
    {
    private:
        std::size_t skippedCharacters{};
        std::size_t skippedBytes{};

    public:
        using CrtpBasicTextIterator<
            TextIteratorWithSkippedCharactersAccumulator>::CrtpBasicTextIterator;

        auto clearAccumulator() noexcept -> void
        {
            skippedBytes = 0;
            skippedCharacters = 0;
        }

        [[nodiscard]] constexpr auto getSkippedCharacters() const noexcept -> std::size_t
        {
            return skippedCharacters;
        }

        [[nodiscard]] constexpr auto getSkippedBytes() const noexcept -> std::size_t
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
      : location{text_iterator.getLocation()}
      , wholeText{text_iterator.getWholeInput()}
    {}

    Token::Token(SmallId token_id)
      : id{token_id}
    {}

    Token::Token(
        const TokenEnvironment &token_environment, isl::string_view token_repr, SmallId token_id)
      : environment{token_environment}
      , repr{token_repr}
      , id{token_id}
    {}

    Token::Token(
        const TokenEnvironment &token_environment, typename isl::string_view::iterator text_begin,
        SmallId token_id)
      : Token{token_environment, {text_begin, isl::as<std::size_t>(0)}, token_id}
    {}

    Token::Token(const text::TextIterator &text_iterator, SmallId token_id)
      : environment{text_iterator}
      , repr{text_iterator.getRemaining()}
      , id{token_id}
    {}

    auto Token::clear(SmallId new_id) noexcept -> void
    {
        id = new_id;
        prefixes.clear();
        postfixes.clear();
    }

    auto Token::getInlineRepr() const noexcept -> isl::string_view
    {
        return text::TextIterator::linesOfFragment(environment.wholeText, repr);
    }

    auto Token::getTabsAndSpaces() const noexcept -> isl::string_view
    {
        const auto whole_input = environment.wholeText;
        const auto *it = std::next(repr.begin(), -1);

        while (it >= whole_input.begin()) {
            if (!isTabOrSpace(*it)) {
                break;
            }

            it = std::next(it, -1);
        }

        return {std::next(it), repr.begin()};
    }

    auto Token::cut(std::size_t first, std::size_t length) const -> Token
    {
        auto new_token = *this;
        new_token.clear(isl::as<std::size_t>(ReservedTokenType::CUT));

        auto text_iterator = TextIteratorWithSkippedCharactersAccumulator(repr);
        text_iterator.skip(first);

        new_token.repr = text_iterator.getRemaining();
        new_token.environment.location = text::Location{
            getFilename(), getLine(), getColumn() + text_iterator.getSkippedCharacters(),
            getRealColumn() + text_iterator.getSkippedBytes()};

        text_iterator.clearAccumulator();
        text_iterator.skip(length);

        new_token.setEnd(new_token.repr.begin() + text_iterator.getSkippedBytes());

        return new_token;
    }

    auto Token::finishInitialization(text::TextIterator &text_iterator, std::size_t totally_skipped)
        -> void
    {
        text_iterator.skip(1);
        repr = text_iterator.getRemainingWithCurrent();
        environment = TokenEnvironment{text_iterator};

        setReprLength(totally_skipped);
        text_iterator.skip(totally_skipped - 1);
    }
}// namespace ccl::lexer
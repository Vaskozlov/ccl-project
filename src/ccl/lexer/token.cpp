#include <ccl/lexer/token.hpp>

namespace ccl::lexer
{
    CCL_INLINE static auto isTabOrSpace(const char chr) noexcept -> bool
    {
        return chr == '\t' || chr == ' ';
    }

    class TextIteratorWithSkippedCharactersAccumulator
      : public text::CrtpBasicTextIterator<TextIteratorWithSkippedCharactersAccumulator>
    {
    private:
        u32 skippedCharacters{};
        u32 skippedBytes{};

    public:
        using CrtpBasicTextIterator::CrtpBasicTextIterator;

        auto clearAccumulator() noexcept -> void
        {
            skippedBytes = 0;
            skippedCharacters = 0;
        }

        [[nodiscard]] constexpr auto getSkippedCharacters() const noexcept -> u32
        {
            return skippedCharacters;
        }

        [[nodiscard]] constexpr auto getSkippedBytes() const noexcept -> u32
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


    Token::Token(const SmallId token_id, const text::InputInfo *input_info)
      : inputInfo{input_info}
      , id{token_id}
    {}


    Token::Token(
        const SmallId token_id, const isl::string_view token_repr,
        const text::Location token_location, const text::InputInfo *input_info)
      : repr{token_repr}
      , location{token_location}
      , inputInfo{input_info}
      , id{token_id}
    {}

    Token::Token(const SmallId token_id, const text::TextIterator &text_iterator)
      : repr{text_iterator.getRemaining()}
      , inputInfo{text_iterator.getInputInfo()}
      , id{token_id}
    {}

    auto Token::clear(const SmallId new_id) noexcept -> void
    {
        id = new_id;
        extractedParts.clear();
    }

    auto Token::getRealColumn() const -> u32
    {
        const auto *it = repr.begin();
        const auto *text_begin = inputInfo->wholeText.begin();

        while (it >= text_begin) {
            if (*it == '\n') {
                break;
            }

            --it;
        }

        return isl::as<u32>(std::distance(it, repr.begin()));
    }

    auto Token::getInlineRepr() const noexcept -> isl::string_view
    {
        return text::TextIterator::linesOfFragment(inputInfo->wholeText, repr);
    }

    auto Token::getTabsAndSpaces() const noexcept -> isl::string_view
    {
        const auto *text_begin = inputInfo->wholeText.begin();
        const auto *it = std::next(repr.begin(), -1);

        while (it >= text_begin) {
            if (!isTabOrSpace(*it)) {
                break;
            }

            it = std::next(it, -1);
        }

        return {std::next(it), repr.begin()};
    }

    auto Token::cut(const std::size_t first, const std::size_t length) const -> Token
    {
        auto new_token = *this;
        new_token.clear(isl::as<std::size_t>(ReservedTokenType::CUT));

        auto text_iterator = TextIteratorWithSkippedCharactersAccumulator(repr);
        text_iterator.skip(first);

        new_token.repr = text_iterator.getRemaining();

        new_token.location = text::Location{
            .line = getLine(),
            .column = getColumn() + text_iterator.getSkippedCharacters(),
        };

        text_iterator.clearAccumulator();
        text_iterator.skip(length);

        new_token.setEnd(new_token.repr.begin() + text_iterator.getSkippedBytes());

        return new_token;
    }

    auto Token::finishInitialization(
        text::TextIterator &text_iterator, const std::size_t totally_skipped) -> void
    {
        text_iterator.skip(1);
        repr = text_iterator.getRemainingWithCurrent();
        location = text_iterator.getLocation();

        setReprLength(totally_skipped);
        text_iterator.skip(totally_skipped - 1);
    }
}// namespace ccl::lexer
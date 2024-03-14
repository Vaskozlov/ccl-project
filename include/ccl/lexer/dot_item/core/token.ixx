export module ccl.lexer.dot_item.core:token;

export import isl;
export import ccl.text;

export namespace ccl::lexer
{
    class TextIteratorWithSkippedChractersAccumulator
        : public text::CrtpBasicTextIterator<TextIteratorWithSkippedChractersAccumulator>
    {
    private:
        std::size_t skippedCharacters{};
        std::size_t skippedBytes{};

    public:
        using CrtpBasicTextIterator<
            TextIteratorWithSkippedChractersAccumulator>::CrtpBasicTextIterator;

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

        [[noreturn]] static auto utfError(char /* chr */) -> void
        {
            throw std::logic_error{"unable to decode utf8 symbol"};
        }
    };

    // NOLINTNEXTLINE
    enum struct ReservedTokenType : Id
    {
        EOI = 0,
        BAD_TOKEN = 1,
        CUT = 2
    };

    constexpr inline std::size_t ReservedTokenMaxValue = 2;

    struct TokenEnvironment
    {
        std::string tabsAndSpaces;
        text::Location location;
        isl::string_view workingLine;

        TokenEnvironment() = default;

        explicit TokenEnvironment(const text::TextIterator &text_iterator)
        : tabsAndSpaces{text_iterator.getTabsAndSpaces()}
        , location{text_iterator.getLocation()}
        , workingLine{text_iterator.getWorkingLine()}
        {}
    };

    class Token
    {
    private:
        std::vector<isl::string_view> prefixes;
        std::vector<isl::string_view> postfixes;
        TokenEnvironment environment;
        isl::string_view repr;
        Id id{};

    public:
        Token() = default;

        [[nodiscard]] explicit Token(Id token_id)
          : id{token_id}
        {}

        [[nodiscard]] Token(
            TokenEnvironment &&token_environment, isl::string_view token_repr, Id token_id)
          : environment{std::move(token_environment)}
          , repr{token_repr}
          , id{token_id}
        {}

        [[nodiscard]] Token(
            TokenEnvironment &&token_environment, typename isl::string_view::iterator text_begin,
            Id token_id)
          : Token{std::move(token_environment), {text_begin, 0ZU}, token_id}
        {}

        [[nodiscard]] Token(const text::TextIterator &text_iterator, Id token_id)
          : environment{text_iterator}
          , repr{text_iterator.getRemaining()}
          , id{token_id}
        {}

        [[nodiscard]] auto getId() const noexcept -> std::size_t
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

        [[nodiscard]] auto getReprSize() const noexcept -> std::size_t
        {
            return repr.size();
        }

        [[nodiscard]] auto getTokenEnvironment() const noexcept -> const TokenEnvironment &
        {
            return environment;
        }

        [[nodiscard]] auto getLocation() const noexcept -> const text::Location &
        {
            return environment.location;
        }

        [[nodiscard]] auto getLine() const noexcept -> std::size_t
        {
            return environment.location.getLine();
        }

        [[nodiscard]] auto getColumn() const noexcept -> std::size_t
        {
            return environment.location.getColumn();
        }

        [[nodiscard]] auto getRealColumn() const noexcept -> std::size_t
        {
            return environment.location.getRealColumn();
        }

        [[nodiscard]] auto getFilename() const noexcept -> isl::string_view
        {
            return environment.location.getFilename();
        }

        [[nodiscard]] auto getRepr() const noexcept -> isl::string_view
        {
            return repr;
        }

        [[nodiscard]] auto getPrefixes() const noexcept -> const std::vector<isl::string_view> &
        {
            return prefixes;
        }

        [[nodiscard]] auto getPostfixes() const noexcept -> const std::vector<isl::string_view> &
        {
            return postfixes;
        }

        [[nodiscard]] auto getInlineRepr() const noexcept -> isl::string_view
        {
            return environment.workingLine;
        }

        [[nodiscard]] auto getTabsAndSpaces() const noexcept -> std::string_view
        {
            return environment.tabsAndSpaces;
        }

        [[nodiscard]] auto cut(std::size_t first, std::size_t length) const -> Token
        {
            auto new_token = *this;
            new_token.clear(isl::as<std::size_t>(ReservedTokenType::CUT));

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

        auto clear(Id new_id) noexcept -> void
        {
            id = new_id;
            prefixes.clear();
            postfixes.clear();
        }

        auto setReprLength(std::size_t length) noexcept -> void
        {
            repr = repr.changeLength<isl::FunctionAPI::UNSAFE>(length);
        }

        auto addPrefix(isl::string_view prefix) -> void
        {
            prefixes.push_back(prefix);
        }

        auto addPostfix(isl::string_view postfix) -> void
        {
            postfixes.push_back(postfix);
        }

        auto setEnd(isl::string_view::iterator end_of_repr) noexcept -> void
        {
            repr = {repr.begin(), end_of_repr};
        }

        auto finishInitialization(text::TextIterator &text_iterator, std::size_t totally_skipped)
            -> void
        {
            text_iterator.skip(1);
            repr = text_iterator.getRemainingWithCurrent();
            environment = TokenEnvironment{text_iterator};

            setReprLength(totally_skipped);
            text_iterator.skip(totally_skipped - 1);
        }
    };
}// namespace ccl::lexer

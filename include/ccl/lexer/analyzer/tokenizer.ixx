export module ccl.lexer.analyzer:tokenizer;

export import ccl.text;
export import ccl.handler;
export import ccl.lexer.dot_item;

export namespace ccl::lexer
{
    class Tokenizer
    {
        std::array<Token, 2> tokens;
        text::TextIterator textIterator;
        std::vector<dot_item::Container> &items;
        dot_item::SpecialItems &specialItems;
        std::vector<Id> &ignoredIds;
        std::string &skippedCharacters;

        bool hasFutureToken{false};
        bool tokenIndex{};

    public:
        [[nodiscard]] Tokenizer(
            std::vector<dot_item::Container> &containers, dot_item::SpecialItems &special_items,
            std::vector<Id> &ignored_ids, std::string &skipped_characters,
            handler::ExceptionHandler &exception_handler, isl::string_view text,
            std::string_view filename = {})
          : textIterator{text, exception_handler, filename}
          , items{containers}
          , specialItems{special_items}
          , ignoredIds{ignored_ids}
          , skippedCharacters{skipped_characters}
        {}

        [[nodiscard]] auto getIterator() const -> const text::TextIterator &
        {
            return textIterator;
        }

        [[nodiscard]] auto getHandler() -> handler::ExceptionHandler &
        {
            return textIterator.getHandler();
        }

        [[nodiscard]] auto getCurrentToken() -> Token &
        {
            return tokens[tokenIndex];// NOLINT
        }

        auto throwExceptionToHandler(
            text::ExceptionCriticality criticality, isl::string_view message,
            isl::string_view suggestion = {}) -> void
        {
            textIterator.throwToHandle(
                textIterator, criticality, text::AnalysisStage::LEXICAL_ANALYSIS, message,
                suggestion);
        }

        auto yield() -> const Token &
        {
            if (hasFutureToken) {
                hasFutureToken = false;
                tokenIndex = !tokenIndex;
            } else {
                nextToken(tokens[tokenIndex]);// NOLINT
            }

            return tokens[tokenIndex];// NOLINT
        }

        auto yieldFutureToken() -> const Token &
        {
            if (!hasFutureToken) {
                nextToken(tokens[!tokenIndex]);// NOLINT
                hasFutureToken = true;
            }

            return tokens[!tokenIndex];// NOLINT
        }

    private:
        auto nextToken(Token &token) -> void
        {
            auto scan_container = [this, &token](const dot_item::Container &container) {
                return container.beginScan(textIterator, token);
            };

            while (skippedCharacters.find(textIterator.getNextCarriageValue()) !=
                   std::string::npos) {
                textIterator.skip(1);
            }

            if (textIterator.isEOI()) [[unlikely]] {
                constructEoiToken(token);
                return;
            }

            if (specialItems.specialScan(textIterator, token)) {
                return returnIfNotInIgnored(token);
            }

            if (std::ranges::any_of(items, scan_container)) {
                return returnIfNotInIgnored(token);
            }

            char next_carriage_value = textIterator.getNextCarriageValue();

            if (text::isLayout(next_carriage_value)) {
                skippedCharacters.push_back(next_carriage_value);
                textIterator.skip(1);
                return nextToken(token);
            }

            constructBadToken(token);

            if (shouldIgnoreToken(token)) [[unlikely]] {
                return nextToken(token);
            }
        }

        auto returnIfNotInIgnored(Token &token) -> void
        {
            if (shouldIgnoreToken(token)) [[unlikely]] {
                return nextToken(token);
            }
        }

        [[nodiscard]] auto shouldIgnoreToken(const Token &token) const -> bool
        {
            return std::ranges::find(ignoredIds, token.getId()) != ignoredIds.end();
        }

        auto constructBadToken(Token &token) -> void
        {
            token = {textIterator, static_cast<Id>(ReservedTokenType::BAD_TOKEN)};

            while (!text::isLayoutOrEoF(textIterator.getNextCarriageValue())) {
                textIterator.advance();
            }

            token.setEnd(textIterator.getRemainingAsCarriage());
        }

        auto constructEoiToken(Token &token) const -> void
        {
            token = {textIterator, static_cast<Id>(ReservedTokenType::EOI)};
        }
    };
}// namespace ccl::lexer

#include <ccl/lexer/tokenizer.hpp>

namespace ccl::lexer
{
    using Tokenizer = LexicalAnalyzer::Tokenizer;

    Tokenizer::Tokenizer(
        const LexicalAnalyzer &lexical_analyzer, isl::string_view text, isl::string_view filename)
      : lexicalAnalyzer{lexical_analyzer}
      , textIterator{text, lexical_analyzer.exceptionHandler, filename}
    {}

    Tokenizer::Tokenizer(
        const LexicalAnalyzer &lexical_analyzer, isl::string_view text, isl::string_view filename,
        ExceptionHandler &exception_handler)
      : lexicalAnalyzer{lexical_analyzer}
      , textIterator{text, exception_handler, filename}
    {}

    auto Tokenizer::shouldIgnoreToken(const Token &token) const -> bool
    {
        const auto &ignored_ids = lexicalAnalyzer.ignoredIds;
        return std::ranges::find(ignored_ids, token.getId()) != ignored_ids.end();
    }

    // NOLINTNEXTLINE (recursive function)
    auto Tokenizer::nextToken(Token &token) -> void
    {
        auto &chars_to_skip = lexicalAnalyzer.skippedCharacters;
        auto scan_container = [this, &token](const std::unique_ptr<Container> &container) {
            return container->beginScan(textIterator, token);
        };

        while (chars_to_skip.find(textIterator.getNextCarriageValue()) != std::string::npos) {
            textIterator.skip(1);
        }

        if (textIterator.isEOI()) [[unlikely]] {
            constructEoiToken(token);
            return;
        }

        if (lexicalAnalyzer.anyPlaceItems.isSuccessfulScan(textIterator, token) ||
            std::ranges::any_of(lexicalAnalyzer.items, scan_container)) {
            return returnIfNotInIgnored(token);
        }

        const char next_carriage_value = textIterator.getNextCarriageValue();

        if (isLayout(next_carriage_value)) {
            chars_to_skip.push_back(next_carriage_value);
            textIterator.skip(1);
            return nextToken(token);
        }

        constructBadToken(token);

        if (shouldIgnoreToken(token)) [[unlikely]] {
            return nextToken(token);
        }
    }

    // NOLINTNEXTLINE (recursive function)
    CCL_INLINE auto LexicalAnalyzer::Tokenizer::returnIfNotInIgnored(Token &token) -> void
    {
        if (shouldIgnoreToken(token)) [[unlikely]] {
            nextToken(token);
        }
    }

    auto Tokenizer::throwExceptionToHandler(
        ExceptionCriticality criticality, isl::string_view message,
        isl::string_view suggestion) const -> void
    {
        textIterator.throwToHandle(
            textIterator, criticality, AnalysisStage::LEXICAL_ANALYSIS, message, suggestion);
    }

    auto Tokenizer::yield() -> const Token &
    {
        if (hasFutureToken) {
            hasFutureToken = false;
            tokenIndex = !tokenIndex;
        } else {
            nextToken(tokens[tokenIndex]);// NOLINT
        }

        return tokens[tokenIndex];// NOLINT
    }

    auto Tokenizer::yieldFutureToken() -> const Token &
    {
        if (!hasFutureToken) {
            nextToken(tokens[!tokenIndex]);// NOLINT
            hasFutureToken = true;
        }

        return tokens[!tokenIndex];// NOLINT
    }

    CCL_INLINE auto Tokenizer::constructEoiToken(Token &token) const -> void
    {
        token = {textIterator, std::to_underlying(ReservedTokenType::EOI)};
    }

    CCL_INLINE auto Tokenizer::constructBadToken(Token &token) -> void
    {
        token = {textIterator, std::to_underlying(ReservedTokenType::BAD_TOKEN)};

        while (!isLayoutOrEoF(textIterator.getNextCarriageValue())) {
            textIterator.advance();
        }

        token.setEnd(textIterator.getRemainingAsCarriage());
    }
}// namespace ccl::lexer

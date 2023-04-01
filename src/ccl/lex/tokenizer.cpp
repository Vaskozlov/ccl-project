#include <ccl/lex/tokenizer.hpp>

using namespace std::string_literals;

namespace ccl::lex
{
    using Tokenizer = LexicalAnalyzer::Tokenizer;

    Tokenizer::Tokenizer(LexicalAnalyzer &lexical_analyzer, string_view text, string_view filename)
      : lexicalAnalyzer{lexical_analyzer}
      , textIterator{text, lexical_analyzer.exceptionHandler, filename}
    {}

    Tokenizer::Tokenizer(
        LexicalAnalyzer &lexical_analyzer, string_view text, string_view filename,
        ExceptionHandler &exception_handler)
      : lexicalAnalyzer{lexical_analyzer}
      , textIterator{text, exception_handler, filename}
    {}

    CCL_INLINE auto Tokenizer::shouldIgnoreToken(const Token &token) const -> bool
    {
        const auto &ignoring_list = lexicalAnalyzer.ignoredIds;
        return ignoring_list.contains(token.getId());
    }

    // NOLINTNEXTLINE (recursive function)
    auto Tokenizer::nextToken(Token &token) -> void
    {
        auto &chars_to_skip = lexicalAnalyzer.skippedCharacters;
        auto scan_container = [this, &token](const Container &container) {
            return container.beginScan(textIterator, token);
        };

        while (chars_to_skip.contains(textIterator.getNextCarriageValue())) {
            textIterator.skip(1);
        }

        if (textIterator.isEOI()) [[unlikely]] {
            constructEoiToken(token);
            return;
        }

        if (lexicalAnalyzer.specialItems.specialScan(textIterator, token)) {
            return returnIfNotInIgnored(token);
        }

        if (std::ranges::any_of(lexicalAnalyzer.items, scan_container)) {
            return returnIfNotInIgnored(token);
        }

        auto next_carriage_value = textIterator.getNextCarriageValue();

        if (isLayout(next_carriage_value)) {
            chars_to_skip.emplace(next_carriage_value);
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
            return nextToken(token);
        }
    }

    auto Tokenizer::throwException(
        ExceptionCriticality criticality, string_view message, string_view suggestion) -> void
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
        token = {textIterator, static_cast<Id>(ReservedTokenType::EOI)};
    }

    CCL_INLINE auto Tokenizer::constructBadToken(Token &token) -> void
    {
        token = {textIterator, static_cast<Id>(ReservedTokenType::BAD_TOKEN)};

        while (!isLayoutOrEoF(textIterator.getNextCarriageValue())) {
            textIterator.next();
        }

        token.setEnd(textIterator.getRemainingAsCarriage());
    }
}// namespace ccl::lex

#include <ccl/lex/lexical_analyzer.hpp>

using namespace std::string_literals;

namespace ccl::lex
{
    using ScanType = dot_item::ScanType;

    CCL_INLINE auto LexicalAnalyzer::Tokenizer::shouldIgnoreToken(const Token &token) const -> bool
    {
        const auto &ignoring_list = lexicalAnalyzer.ignoredIds;
        return ignoring_list.contains(token.getId());
    }

    // NOLINTNEXTLINE (recursive function)
    auto LexicalAnalyzer::Tokenizer::nextToken(Token &token) -> void
    {
        auto &chars_to_skip = lexicalAnalyzer.skippedCharacters;
        auto scan_container = [this, &token](const auto &container) {
            return container.beginScan(textIterator, token);
        };

        while (chars_to_skip.contains(textIterator.getNextCarriageValue())) {
            textIterator.skip(1);
        }

        if (textIterator.isEOI()) [[unlikely]] {
            constructEOIToken(token);
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
            return nextToken(token);
        }
    }

    LexicalAnalyzer::Tokenizer::Tokenizer(
        LexicalAnalyzer &lexical_analyzer_, string_view text, string_view filename_)
      : lexicalAnalyzer{lexical_analyzer_}
      , textIterator{text, lexical_analyzer_.exceptionHandler, filename_}
    {}

    LexicalAnalyzer::Tokenizer::Tokenizer(
        LexicalAnalyzer &lexical_analyzer_, string_view text, string_view filename_,
        ExceptionHandler &exception_handler_)
      : lexicalAnalyzer{lexical_analyzer_}
      , textIterator{text, exception_handler_, filename_}
    {}

    auto LexicalAnalyzer::Tokenizer::throwException(
        ExceptionCriticality criticality, string_view message, string_view suggestion) -> void
    {
        textIterator.throwToHandle(
            textIterator, criticality, AnalysationStage::LEXICAL_ANALYSIS, message, suggestion);
    }

    auto LexicalAnalyzer::Tokenizer::yield() -> Token &
    {
        if (hasFutureToken) {
            hasFutureToken = false;
            currentToken = std::move(futureToken);
        } else {
            nextToken(currentToken);
        }

        return currentToken;
    }

    auto LexicalAnalyzer::Tokenizer::yieldFutureToken() -> Token &
    {
        if (!hasFutureToken) {
            nextToken(futureToken);
            hasFutureToken = true;
        }

        return futureToken;
    }

    CCL_INLINE auto LexicalAnalyzer::Tokenizer::constructEOIToken(Token &token) -> void
    {
        token = {textIterator, ReservedTokenType::EOI};
    }

    CCL_INLINE auto LexicalAnalyzer::Tokenizer::constructBadToken(Token &token) -> void
    {
        token = Token{textIterator, ReservedTokenType::BAD_TOKEN};

        while (!isLayoutOrEoF(textIterator.getNextCarriageValue())) {
            textIterator.next();
        }

        token.setEnd(textIterator.getRemainingAsCarriage());
    }
}// namespace ccl::lex

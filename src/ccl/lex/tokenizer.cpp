#include <ccl/lex/lexical_analyzer.hpp>

using namespace std::string_literals;

namespace ccl::lex
{
    using ScanType = dot_item::ScanType;

    CCL_INLINE auto LexicalAnalyzer::Tokenizer::shouldIgnoreToken(const Token &token) const -> bool
    {
        const auto &ignoring_list = lexical_analyzer.ignored_ids;
        return ignoring_list.contains(token.getId());
    }

    // NOLINTNEXTLINE (recursive function)
    auto LexicalAnalyzer::Tokenizer::nextToken(Token &token) -> void
    {
        auto &chars_to_skip = lexical_analyzer.skipped_characters;
        auto container_scan = [this, &token](const auto &container) {
            return container.beginScan(text_iterator, token);
        };

        while (chars_to_skip.contains(text_iterator.getNextCarriageValue())) {
            text_iterator.skip(1);
        }

        if (text_iterator.isEOI()) [[unlikely]] {
            constructEOIToken(token);
            return;
        }

        if (lexical_analyzer.special_items.specialScan(text_iterator, token)) {
            return returnIfNotInIgnored(token);
        }

        if (std::ranges::any_of(lexical_analyzer.items, container_scan)) {
            return returnIfNotInIgnored(token);
        }

        auto next_carriage_value = text_iterator.getNextCarriageValue();

        if (isLayout(next_carriage_value)) {
            chars_to_skip.push_back(next_carriage_value);
            text_iterator.skip(1);
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

    auto LexicalAnalyzer::Tokenizer::yield() -> Token &
    {
        if (has_future_token) {
            has_future_token = false;
            current_token = std::move(future_token);
        } else {
            nextToken(current_token);
        }

        return current_token;
    }

    auto LexicalAnalyzer::Tokenizer::futureToken() -> Token &
    {
        if (not has_future_token) {
            nextToken(future_token);
            has_future_token = true;
        }

        return future_token;
    }

    CCL_INLINE auto LexicalAnalyzer::Tokenizer::constructEOIToken(Token &token) -> void
    {
        token = { text_iterator, ReservedTokenType::EOI };
    }

    CCL_INLINE auto LexicalAnalyzer::Tokenizer::constructBadToken(Token &token) -> void
    {
        token = Token{ text_iterator, ReservedTokenType::BAD_TOKEN };

        while (not isLayoutOrEoF(text_iterator.getNextCarriageValue())) {
            text_iterator.next();
        }

        token.setEnd(text_iterator.getRemainingAsCarriage());
    }
}// namespace ccl::lex

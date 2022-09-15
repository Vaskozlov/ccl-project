#include <ccl/lex/lexical_analyzer.hpp>

using namespace std::string_literals;

namespace ccl::lex
{
    using ScanType = dot_item::ScanType;

    CCL_INLINE auto LexicalAnalyzer::Tokenizer::shouldIgnoreToken(const Token &token) const -> bool
    {
        const auto &ignore = lexical_analyzer.ignored_ids;
        return ignore.contains(token.getId());
    }

    auto LexicalAnalyzer::Tokenizer::nextToken(Token &token) -> void
    {
        auto &chars_to_skip = lexical_analyzer.skipped_characters;

        while (true) {
            while (chars_to_skip.contains(text_iterator.getNextCarriageValue())) {
                text_iterator.skip(1);
            }

            if (text_iterator.isEOI()) [[unlikely]] {
                constructEOIToken(token);
                break;
            }

            auto special_item = lexical_analyzer.special_items.specialScan(text_iterator, token);

            if (special_item) {
                goto EndYielding;// NOLINT
            }

            for (auto &&container : lexical_analyzer.items) {
                auto scan_result = container.beginScan(text_iterator, token);

                if (scan_result) {
                    goto EndYielding;// NOLINT
                }
            }

            if (isLayout(text_iterator.getNextCarriageValue())) {
                chars_to_skip.push_back(text_iterator.getNextCarriageValue());
                text_iterator.skip(1);
                continue;
            }

            constructBadToken(token);

        EndYielding:
            if (shouldIgnoreToken(token)) {
                continue;
            }

            break;
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

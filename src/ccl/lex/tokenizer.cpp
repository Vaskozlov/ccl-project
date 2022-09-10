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

    auto LexicalAnalyzer::Tokenizer::yield() -> Token &
    {
        while (true) {
            if (text_iterator.isEOI()) [[unlikely]] {
                constructEOIToken();
                return local_token;
            }

            auto special_item =
                lexical_analyzer.special_items.specialScan(text_iterator, local_token);

            if (special_item) {
                goto EndYielding;// NOLINT
            }

            for (auto &&container : lexical_analyzer.items) {
                auto scan_result = container.beginScan(text_iterator, local_token);

                if (scan_result) {
                    goto EndYielding;// NOLINT
                }
            }

            if (isLayout(text_iterator.getNextCarriageValue())) {
                text_iterator.skip(1);
                continue;
            }

            constructBadToken();

        EndYielding:
            if (lexical_analyzer.ignored_ids.contains(local_token.getId())) {
                continue;
            }

            return local_token;
        }
    }

    CCL_INLINE auto LexicalAnalyzer::Tokenizer::constructEOIToken() -> void
    {
        local_token = { text_iterator, ReservedTokenType::EOI };
    }

    CCL_INLINE auto LexicalAnalyzer::Tokenizer::constructBadToken() -> void
    {
        local_token = Token{ text_iterator, ReservedTokenType::BAD_TOKEN };

        while (not isLayoutOrEoF(text_iterator.getNextCarriageValue())) {
            text_iterator.next();
        }

        local_token.setEnd(text_iterator.getRemainingAsCarriage());
    }
}// namespace ccl::lex

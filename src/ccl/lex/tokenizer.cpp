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

    auto LexicalAnalyzer::Tokenizer::yield() -> Token
    {
        if (text_iterator.isEOI()) {
            return constructEOIToken();
        }

        auto token = Token{ text_iterator, 0 };
        auto special_item = lexical_analyzer.special_items.specialScan(text_iterator);

        if (special_item.has_value()) {
            return *special_item;
        }

        for (auto &&container : lexical_analyzer.items) {
            auto scan_result = container.beginScan(text_iterator);

            if (scan_result.has_value()) {
                return *scan_result;
            }
        }

        if (isLayout(text_iterator.getNextCarriageValue())) {
            text_iterator.skip(1);
            return yield();
        }

        return constructBadToken();
    }

    CCL_INLINE auto LexicalAnalyzer::Tokenizer::constructEOIToken() -> Token
    {
        return { text_iterator, ReservedTokenType::EOI };
    }

    CCL_INLINE auto LexicalAnalyzer::Tokenizer::constructBadToken() -> Token
    {
        auto token = Token{ text_iterator, ReservedTokenType::BAD_TOKEN };

        while (not isLayoutOrEoF(text_iterator.getNextCarriageValue())) {
            text_iterator.next();
        }

        token.setEnd(text_iterator.getRemainingAsCarriage());
        return token;
    }
}// namespace ccl::lex

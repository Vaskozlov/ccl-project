#include <ccl/lex/lexical_analyzer.hpp>

using namespace std::string_literals;

namespace ccl::lex
{
    using ScanType = dot_item::ScanType;

    auto LexicalAnalyzer::Tokenizer::shouldIgnoreToken(const Token &token) const -> bool
    {
        const auto &ignore = lexical_analyzer.ignored_ids;
        return ignore.contains(token.getId());
    }

    auto LexicalAnalyzer::Tokenizer::yield() -> Token
    {
        const auto &special = lexical_analyzer.special_items;
        const auto &containers = lexical_analyzer.items;

        while (true) {
        ScanBegin:

            if (text_iterator.isEnd()) {
                return constructEOIToken();
            }

            auto special_result = special.scan(text_iterator);

            if (special_result.has_value()) {
                auto &[iterator, token] = *special_result;

                text_iterator = std::move(iterator);

                if (shouldIgnoreToken(token) || token.emptyRepr()) {
                    continue;
                }

                return token;
            }

            for (auto &&container : containers) {
                auto scan_result = container.scan(text_iterator, ScanType::MAIN);

                if (scan_result.has_value()) {
                    auto &[iterator, token] = *scan_result;

                    text_iterator = std::move(iterator);

                    if (shouldIgnoreToken(token)) {
                        goto ScanBegin;// NOLINT
                    }

                    if (token.emptyRepr()) {
                        continue;
                    }

                    return token;
                }
            }

            auto next_carriage_value = text_iterator.getNextCarriageValue();

            if (isLayout(next_carriage_value)) {
                text_iterator.next();
                continue;
            }

            return constructBadToken();
        }
    }

    auto LexicalAnalyzer::Tokenizer::constructEOIToken() -> Token
    {
        return { text_iterator, ReservedTokenType::EOI, "$"s };
    }

    auto LexicalAnalyzer::Tokenizer::constructBadToken() -> Token
    {
        auto token = Token{ text_iterator, ReservedTokenType::BAD_TOKEN };

        while (not isLayoutOrEoF(text_iterator.getNextCarriageValue())) {
            text_iterator.next();
        }

        token.setEnd(text_iterator.getRemainingAsCarriage());
        return token;
    }
}// namespace ccl::lex

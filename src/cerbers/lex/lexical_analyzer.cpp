#include <cerberus/lex/lexical_analyzer.hpp>

namespace cerb::lex
{
    auto LexicalAnalyzer::getTokenizer(TextIterator &text_iterator) const
        -> coro::Generator<std::optional<Token>>
    {
        while (true) {
            co_yield yield(text_iterator);
        }
    }

    LexicalAnalyzer::LexicalAnalyzer(
        const std::initializer_list<std::pair<size_t, u8string_view>> &rules_,
        u8string_view filename, CommentTokens comment_tokens_)
    {
        for (auto [id, rule] : rules_) {
            errors += createDotItem(rule, id, comment_tokens_, filename);
        }
    }

    auto LexicalAnalyzer::yield(TextIterator &text_iterator) const -> std::optional<Token>
    {
        for (const auto &item : items) {
            text_iterator.skipCommentsAndLayout();

            auto special_token = shared.getSpecialToken(text_iterator);

            if (special_token.has_value()) {
                return *special_token;
            }

            auto scan_result = item.scan(text_iterator, Token{ text_iterator, item.getId() }, true);

            if (scan_result) {
                auto &[iterator, token] = *scan_result;
                text_iterator = std::move(iterator);
                return token;
            }
        }

        return std::nullopt;
    }

    auto LexicalAnalyzer::createDotItem(
        u8string_view rule, size_t id, const CommentTokens &comment_tokens, u8string_view filename)
        -> size_t
    {
        items.emplace_back(
            TextIterator{ rule, &exception_accumulator, comment_tokens, filename }, id, shared);
        return 0;
    }
}// namespace cerb::lex

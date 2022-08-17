#include <cerberus/lex/lexical_analyzer.hpp>

namespace cerb::lex
{
    auto LexicalAnalyzer::getTokenizer(TextIterator &text_iterator) const -> coro::Generator<Token>
    {
        while (true) {
            if (auto token = yield(text_iterator); token) {
                co_yield token;
            } else {
                break;
            }
        }

        co_yield Token{ text_iterator, 0 };
    }

    LexicalAnalyzer::LexicalAnalyzer(
        const std::initializer_list<std::pair<size_t, u8string_view>> &rules_,
        u8string_view filename, CommentTokens comment_tokens_)
    {
        for (auto [id, rule] : rules_) {
            errors += createDotItem(rule, id, comment_tokens_, filename);
        }
    }

    auto LexicalAnalyzer::yield(TextIterator &text_iterator) const -> Token
    {
        for (const auto &item : items) {
            text_iterator.skipCommentsAndLayout();

            auto scan_result = item.scan(text_iterator, Token{ text_iterator, item.getId() }, true);

            if (scan_result) {
                auto &[iterator, token] = *scan_result;
                text_iterator = std::move(iterator);
                return token;
            }
        }

        return Token{};
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

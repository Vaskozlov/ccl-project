#include <cerberus/lex/lexical_analyzer.hpp>

namespace cerb::lex
{
    LexicalAnalyzer::LexicalAnalyzer(
        const std::initializer_list<std::pair<size_t, u8string_view>> &rules_,
        u8string_view filename, const CommentTokens &comment_tokens_)
    {
        for (const auto &[id, rule] : rules_) {
            errors += createDotItem(rule, id, comment_tokens_, filename);
        }
    }

    auto LexicalAnalyzer::yield(TextIterator &text_iterator) const -> Token
    {
        if (text_iterator.isEnd()) {
            return Token{};
        }

        for (auto &&item : items) {
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

        text_iterator.throwException<LexicalAnalysisException>(u8"unrecognizable token");
        throw UnrecoverableError{ "unrecognizable token" };// TODO: return token with error id
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

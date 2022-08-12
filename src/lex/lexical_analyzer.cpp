#include <cerberus/lex/lexical_analyzer.hpp>

namespace cerb::lex
{
    auto LexicalAnalyzer::yield(TextIterator &text_iterator) const -> void
    {
        for (const auto &item : items) {
            text_iterator.skipCommentsAndLayout();

            auto [success, iterator] = item.scan(text_iterator, true);
            ::fmt::print("{}\n", success);
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

    auto LexicalAnalyzer::createDotItem(
        u8string_view rule, size_t id, const CommentTokens &comment_tokens, u8string_view filename)
        -> size_t
    {
        items.emplace_back(
            TextIterator{ rule, &exception_accumulator, comment_tokens, filename }, id, shared);
        return 0;
    }
}// namespace cerb::lex

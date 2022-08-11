#ifndef CERBERUS_PROJECT_LEXICAL_ANALYZER_HPP
#define CERBERUS_PROJECT_LEXICAL_ANALYZER_HPP

#include <cerberus/lex/dot_item/dot_item.hpp>
#include <deque>
#include <fmt/format.h>

namespace cerb::lex
{
    class LexicalAnalyzer
    {
        using DotItem = dot_item::DotItem;
        using BasicItem = dot_item::BasicItem;
        using TextIterator = typename BasicItem::TextIterator;
        using CommentTokens = typename BasicItem::CommentTokens;
        using ExceptionAccumulator = typename BasicItem::ExceptionAccumulator;

    public:
        auto yield(TextIterator &text_iterator) const -> void
        {
            for (const auto &item : items) {
                text_iterator.skipCommentsAndLayout();

                auto [success, iterator] = item.scan(text_iterator, true);
                ::fmt::print(
                    "{}\n{}\n", success,
                    (const char *)iterator.getRemainingFutureAfterSymbols(1).begin());
            }
        }

        LexicalAnalyzer(
            const std::initializer_list<std::pair<size_t, u8string_view>> &rules_,
            u8string_view filename = {}, CommentTokens comment_tokens_ = { u8"#" })
        {
            for (auto [id, rule] : rules_) {
                errors += createDotItem(rule, id, comment_tokens_, filename);
            }
        }

    private:
        auto createDotItem(
            u8string_view rule, size_t id, const CommentTokens &comment_tokens,
            u8string_view filename) -> size_t
        {
            items.emplace_back(
                TextIterator{ rule, &exception_accumulator, comment_tokens, filename }, id, shared);
            return 0;
        }

        std::deque<DotItem> items{};
        AnalysisShared shared{};
        ExceptionAccumulator exception_accumulator{};
        size_t errors{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_PROJECT_LEXICAL_ANALYZER_HPP */

#ifndef CERBERUS_PROJECT_LEXICAL_ANALYZER_HPP
#define CERBERUS_PROJECT_LEXICAL_ANALYZER_HPP

#include <cerberus/lex/dot_item/dot_item.hpp>
#include <deque>
#include <fmt/format.h>

namespace cerb::lex
{
    template<CharacterLiteral CharT>
    class LexicalAnalyzer
    {
        using TextIterator = typename dot_item::BasicItem<CharT>::TextIterator;
        using CommentTokens = typename dot_item::BasicItem<CharT>::CommentTokens;
        using ExceptionAccumulator = typename dot_item::BasicItem<CharT>::ExceptionAccumulator;

    public:
        constexpr auto yield(TextIterator &text_iterator) const -> void
        {
            for (auto &item : items) {
                text_iterator.skipCommentsAndLayout();
                auto result = item.scan(text_iterator);
                ::fmt::print("{}\n{}\n", result.first, result.second.getRemainingFuture(1));
            }
        }

        constexpr LexicalAnalyzer(
            const std::initializer_list<std::pair<size_t, StrView<CharT>>> &rules_,
            CommentTokens comment_tokens_ = { "#", {}, {} }, string_view filename = {})
        {
            for (auto [id, rule] : rules_) {
                errors += createDotItem(rule, id, comment_tokens_, filename);
            }
        }

    private:
        constexpr auto createDotItem(
            StrView<CharT> rule, size_t id, const CommentTokens &comment_tokens,
            string_view filename) -> size_t
        {
            items.emplace_back(
                TextIterator{ rule, &exception_accumulator, comment_tokens, filename }, id, shared);
            return 0;
        }


        std::deque<dot_item::DotItem<CharT>> items{};
        AnalysisShared<CharT> shared{};
        ExceptionAccumulator exception_accumulator{};
        size_t errors{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_PROJECT_LEXICAL_ANALYZER_HPP */

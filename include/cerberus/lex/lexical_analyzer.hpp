#ifndef CERBERUS_PROJECT_LEXICAL_ANALYZER_HPP
#define CERBERUS_PROJECT_LEXICAL_ANALYZER_HPP

#include <cerberus/lex/dot_item/dot_item.hpp>
#include <deque>

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
        LexicalAnalyzer(
            const std::initializer_list<std::pair<size_t, u8string_view>> &rules_,
            u8string_view filename = {}, const CommentTokens &comment_tokens_ = { u8"#" });

        [[nodiscard]] auto getExceptionAccumulator() -> ExceptionAccumulator &
        {
            return exception_accumulator;
        }

        [[nodiscard]] auto getExceptionAccumulator() const -> const ExceptionAccumulator &
        {
            return exception_accumulator;
        }

        auto yield(TextIterator &text_iterator) const -> Token;

    private:
        auto createDotItem(
            u8string_view rule, size_t id, const CommentTokens &comment_tokens,
            u8string_view filename) -> size_t;

        std::deque<DotItem> items{};
        AnalysisShared shared{};
        ExceptionAccumulator exception_accumulator{};
        size_t errors{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_PROJECT_LEXICAL_ANALYZER_HPP */

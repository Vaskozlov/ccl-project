#ifndef CCL_PROJECT_LEXICAL_ANALYZER_HPP
#define CCL_PROJECT_LEXICAL_ANALYZER_HPP

#include <ccl/lex/dot_item/dot_item.hpp>
#include <deque>

namespace ccl::lex
{
    class LexicalAnalyzer
    {
    public:
        using DotItem = dot_item::DotItem;
        using BasicItem = dot_item::BasicItem;
        using TextIterator = typename BasicItem::TextIterator;
        using CommentTokens = typename BasicItem::CommentTokens;
        using ExceptionAccumulator = typename BasicItem::ExceptionAccumulator;

        struct Tokenizer
        {
            Tokenizer(LexicalAnalyzer &lexical_analyzer_, u8string_view text)
              : lexical_analyzer(lexical_analyzer_),
                text_iterator(
                    text, &lexical_analyzer_.exception_accumulator,
                    lexical_analyzer_.shared.comment_tokens)
            {}

            Tokenizer(
                LexicalAnalyzer &lexical_analyzer_, u8string_view text,
                ExceptionAccumulator *exception_accumulator_)
              : lexical_analyzer(lexical_analyzer_),
                text_iterator(text, exception_accumulator_, lexical_analyzer_.shared.comment_tokens)
            {}

            [[nodiscard]] auto yield() -> Token;
            [[nodiscard]] auto constructBadToken() -> Token;

        private:
            const LexicalAnalyzer &lexical_analyzer;
            TextIterator text_iterator;
        };

        LexicalAnalyzer(
            const std::initializer_list<std::pair<size_t, u8string_view>> &rules_,
            u8string_view filename = {}, const CommentTokens &comment_tokens_ = { u8"#" });

        [[nodiscard]] auto getTokenizer(u8string_view text) -> Tokenizer
        {
            return { *this, text };
        }

        [[nodiscard]] auto getTokenizer(u8string_view text, ExceptionAccumulator *accumulator)
            -> Tokenizer
        {
            return { *this, text, accumulator };
        }

        [[nodiscard]] auto getExceptionAccumulator() -> ExceptionAccumulator &
        {
            return exception_accumulator;
        }

        [[nodiscard]] auto getExceptionAccumulator() const -> const ExceptionAccumulator &
        {
            return exception_accumulator;
        }

    private:
        auto createDotItem(
            u8string_view rule, size_t id, const CommentTokens &comment_tokens,
            u8string_view filename) -> size_t;

        std::deque<DotItem> items{};
        AnalysisShared shared{};
        ExceptionAccumulator exception_accumulator{};
        size_t errors{};
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_LEXICAL_ANALYZER_HPP */

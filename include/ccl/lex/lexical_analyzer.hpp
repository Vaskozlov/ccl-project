#ifndef CCL_PROJECT_LEXICAL_ANALYZER_HPP
#define CCL_PROJECT_LEXICAL_ANALYZER_HPP

#include <ccl/lex/dot_item/container.hpp>
#include <set>

namespace ccl::lex
{
    class LexicalAnalyzer
    {
    public:
        using Container = dot_item::Container;
        using BasicItem = dot_item::BasicItem;
        using TextIterator = typename BasicItem::TextIterator;
        using CommentTokens = typename BasicItem::CommentTokens;

        struct Tokenizer
        {
            Tokenizer(
                LexicalAnalyzer &lexical_analyzer_, string_view text, string_view filename_ = {})
              : lexical_analyzer(lexical_analyzer_),
                text_iterator(
                    text, exception_handler, lexical_analyzer_.shared.comment_tokens, filename_)
            {}

            Tokenizer(
                LexicalAnalyzer &lexical_analyzer_, string_view text, string_view filename_,
                ExceptionHandler &exception_handler_)
              : lexical_analyzer(lexical_analyzer_),
                text_iterator(
                    text, exception_handler_, lexical_analyzer_.shared.comment_tokens, filename_)
            {}

            template<Exception T = text::TextIteratorException, typename... Args>
            auto throwError(Args &&...args) -> void
            {
                text_iterator.throwError<T>(std::forward<Args>(args)...);
            }

            template<Exception T = text::TextIteratorException, typename... Args>
            auto throwWarning(Args &&...args) -> void
            {
                text_iterator.throwWarning<T>(std::forward<Args>(args)...);
            }

            [[nodiscard]] auto yield() -> Token;
            [[nodiscard]] auto constructBadToken() -> Token;

        private:
            LexicalAnalyzer &lexical_analyzer;
            ExceptionHandler &exception_handler{ lexical_analyzer.exception_handler };
            TextIterator text_iterator;
        };

        LexicalAnalyzer(
            ExceptionHandler &exception_handler_,
            const std::initializer_list<std::pair<size_t, string_view>> &rules_,
            string_view filename = {}, const CommentTokens &comment_tokens_ = { "#" });

        [[nodiscard]] auto getTokenizer(string_view text, string_view filename = {}) -> Tokenizer
        {
            return { *this, text, filename };
        }

        [[nodiscard]] auto
            getTokenizer(string_view text, string_view filename, ExceptionHandler &handler)
                -> Tokenizer
        {
            return { *this, text, filename, handler };
        }

    private:
        auto createContainer(
            string_view rule, size_t id, const CommentTokens &comment_tokens, string_view filename)
            -> void;

        std::set<Container> items;
        AnalysisShared shared{};
        ExceptionHandler &exception_handler;
        size_t errors{};
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_LEXICAL_ANALYZER_HPP */

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

        struct Tokenizer
        {
            Tokenizer(
                LexicalAnalyzer &lexical_analyzer_, string_view text, string_view filename_ = {})
              : lexical_analyzer(lexical_analyzer_),
                text_iterator(text, lexical_analyzer_.exception_handler, filename_)
            {}

            Tokenizer(
                LexicalAnalyzer &lexical_analyzer_, string_view text, string_view filename_,
                ExceptionHandler &exception_handler_)
              : lexical_analyzer(lexical_analyzer_),
                text_iterator(text, exception_handler_, filename_)
            {}

            [[nodiscard]] auto getIterator() const -> const TextIterator &
            {
                return text_iterator;
            }

            [[nodiscard]] auto getExceptionHandler() -> ExceptionHandler &
            {
                return text_iterator.getExceptionHandler();
            }

            auto throwException(
                ExceptionCriticality criticality, string_view message, string_view suggestion = {})
                -> void
            {
                text_iterator.throwToHandle(text_iterator, criticality, message, suggestion);
            }

            [[nodiscard]] auto yield() -> Token;
            [[nodiscard]] auto constructBadToken() -> Token;

        private:
            LexicalAnalyzer &lexical_analyzer;
            TextIterator text_iterator;
        };

        LexicalAnalyzer(
            ExceptionHandler &exception_handler_,
            const std::initializer_list<std::pair<size_t, string_view>> &rules_,
            string_view filename = {});

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
        auto createContainer(string_view rule, size_t id, string_view filename) -> void;

        std::set<Container> items;
        SpecialItems special_items{};
        ExceptionHandler &exception_handler;
        size_t errors{};
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_LEXICAL_ANALYZER_HPP */

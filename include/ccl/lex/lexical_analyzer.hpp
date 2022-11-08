#ifndef CCL_PROJECT_LEXICAL_ANALYZER_HPP
#define CCL_PROJECT_LEXICAL_ANALYZER_HPP

#include <ccl/lex/dot_item/container.hpp>

namespace ccl::lex
{
    class LexicalAnalyzer
    {
    private:
        using Container = dot_item::Container;
        using BasicItem = dot_item::BasicItem;
        using TextIterator = typename BasicItem::TextIterator;

        struct CCL_TRIVIAL_ABI Rule
        {
            size_t id{};
            string_view repr{};
        };

        Vector<Container> items{};
        SpecialItems specialItems{};
        std::basic_string<size_t> ignoredIds{};
        std::string skippedCharacters{};
        ExceptionHandler &exceptionHandler;

    public:
        class Tokenizer;

        LexicalAnalyzer(
            ExceptionHandler &exception_handler_, InitializerList<Rule> rules_,
            string_view filename = {}, std::basic_string<size_t> ignored_ids_ = {});

        [[nodiscard]] auto getIgnoredIds() const -> const std::basic_string<size_t> &
        {
            return ignoredIds;
        }

        [[nodiscard]] auto getTokenizer(string_view text, string_view filename = {}) -> Tokenizer;

        [[nodiscard]] auto
            getTokenizer(string_view text, string_view filename, ExceptionHandler &handler)
                -> Tokenizer;

    private:
        auto createContainer(string_view rule, size_t id, string_view filename) -> void;
    };

    class LexicalAnalyzer::Tokenizer
    {
        Token current_token{};
        Token future_token{};
        LexicalAnalyzer &lexical_analyzer;
        TextIterator text_iterator;
        bool has_future_token{ false };

    public:
        Tokenizer(LexicalAnalyzer &lexical_analyzer_, string_view text, string_view filename_ = {});

        Tokenizer(
            LexicalAnalyzer &lexical_analyzer_, string_view text, string_view filename_,
            ExceptionHandler &exception_handler_);

        [[nodiscard]] auto getIterator() const -> const TextIterator &
        {
            return text_iterator;
        }

        [[nodiscard]] auto getHandler() -> ExceptionHandler &
        {
            return text_iterator.getHandler();
        }

        [[nodiscard]] auto getCurrentToken() -> Token &
        {
            return current_token;
        }

        auto throwException(
            ExceptionCriticality criticality, string_view message, string_view suggestion = {})
            -> void;

        auto yield() -> Token &;
        auto futureToken() -> Token &;

    private:
        auto nextToken(Token &token) -> void;
        auto returnIfNotInIgnored(Token &token) -> void;

        [[nodiscard]] auto shouldIgnoreToken(const Token &token) const -> bool;

        auto constructBadToken(Token &token) -> void;
        auto constructEOIToken(Token &token) -> void;
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_LEXICAL_ANALYZER_HPP */

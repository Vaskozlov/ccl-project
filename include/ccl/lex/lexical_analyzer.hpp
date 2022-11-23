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
            Id id{};
            string_view repr{};
        };

        Vector<Container> items{};
        SpecialItems specialItems{};
        std::basic_string<Id> ignoredIds{};
        std::string skippedCharacters{};
        ExceptionHandler &exceptionHandler;

    public:
        class Tokenizer;

        LexicalAnalyzer(
            ExceptionHandler &exception_handler_, InitializerList<Rule> rules_,
            string_view filename = {}, std::basic_string<Id> ignored_ids_ = {});

        [[nodiscard]] auto getIgnoredIds() const -> const std::basic_string<Id> &
        {
            return ignoredIds;
        }

        [[nodiscard]] auto getTokenizer(string_view text, string_view filename = {}) -> Tokenizer;

        [[nodiscard]] auto
            getTokenizer(string_view text, string_view filename, ExceptionHandler &handler)
                -> Tokenizer;

    private:
        auto createContainer(string_view rule, Id id, string_view filename) -> void;
    };

    class LexicalAnalyzer::Tokenizer
    {
        Token currentToken{};
        Token futureToken{};
        LexicalAnalyzer &lexicalAnalyzer;
        TextIterator textIterator;
        bool hasFutureToken{false};

    public:
        Tokenizer(LexicalAnalyzer &lexical_analyzer_, string_view text, string_view filename_ = {});

        Tokenizer(
            LexicalAnalyzer &lexical_analyzer_, string_view text, string_view filename_,
            ExceptionHandler &exception_handler_);

        [[nodiscard]] auto getIterator() const -> const TextIterator &
        {
            return textIterator;
        }

        [[nodiscard]] auto getHandler() -> ExceptionHandler &
        {
            return textIterator.getHandler();
        }

        [[nodiscard]] auto getCurrentToken() -> Token &
        {
            return currentToken;
        }

        auto throwException(
            ExceptionCriticality criticality, string_view message, string_view suggestion = {})
            -> void;

        auto yield() -> Token &;
        auto yieldFutureToken() -> Token &;

    private:
        auto nextToken(Token &token) -> void;
        auto returnIfNotInIgnored(Token &token) -> void;

        [[nodiscard]] auto shouldIgnoreToken(const Token &token) const -> bool;

        auto constructBadToken(Token &token) -> void;
        auto constructEOIToken(Token &token) -> void;
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_LEXICAL_ANALYZER_HPP */

#ifndef CCL_PROJECT_TOKENIZER_HPP
#define CCL_PROJECT_TOKENIZER_HPP

#include <ccl/lex/lexical_analyzer.hpp>

namespace ccl::lex
{
    class LexicalAnalyzer::Tokenizer
    {
        std::array<Token, 2> tokens;
        LexicalAnalyzer &lexicalAnalyzer;
        TextIterator textIterator;
        bool hasFutureToken{false};
        bool tokenIndex{};

    public:
        [[nodiscard]] Tokenizer(
            LexicalAnalyzer &lexical_analyzer, string_view text, std::string_view filename = {});

        [[nodiscard]] Tokenizer(
            LexicalAnalyzer &lexical_analyzer, string_view text, std::string_view filename,
            ExceptionHandler &exception_handler);

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
            return tokens[tokenIndex];// NOLINT
        }

        auto throwExceptionToHandler(
            ExceptionCriticality criticality, string_view message, string_view suggestion = {})
            -> void;

        auto yield() -> const Token &;
        auto yieldFutureToken() -> const Token &;

    private:
        auto nextToken(Token &token) -> void;
        auto returnIfNotInIgnored(Token &token) -> void;

        [[nodiscard]] auto shouldIgnoreToken(const Token &token) const -> bool;

        auto constructBadToken(Token &token) -> void;
        auto constructEoiToken(Token &token) const -> void;
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_TOKENIZER_HPP */

#ifndef CCL_PROJECT_TOKENIZER_HPP
#define CCL_PROJECT_TOKENIZER_HPP

#include <ccl/lexer/lexical_analyzer.hpp>

namespace ccl::lexer
{
    class LexicalAnalyzer::Tokenizer
    {
        std::array<Token, 2> tokens;
        // NOLINTNEXTLINE reference
        const LexicalAnalyzer &lexicalAnalyzer;
        TextIterator textIterator;
        bool hasFutureToken{};
        bool tokenIndex{};

    public:
        [[nodiscard]] Tokenizer(
            const LexicalAnalyzer &lexical_analyzer, isl::string_view text,
            isl::string_view filename = {});

        [[nodiscard]] Tokenizer(
            const LexicalAnalyzer &lexical_analyzer, isl::string_view text,
            isl::string_view filename, ExceptionHandler &exception_handler);

        [[nodiscard]] auto getIterator() const CCL_LIFETIMEBOUND -> const TextIterator &
        {
            return textIterator;
        }

        [[nodiscard]] auto getHandler() CCL_LIFETIMEBOUND -> ExceptionHandler &
        {
            return textIterator.getHandler();
        }

        [[nodiscard]] auto getCurrentToken() CCL_LIFETIMEBOUND -> Token &
        {
            return tokens[tokenIndex];// NOLINT
        }

        [[nodiscard]] auto getCurrentToken() const CCL_LIFETIMEBOUND -> const Token &
        {
            return tokens[tokenIndex];// NOLINT
        }

        auto throwExceptionToHandler(
            ExceptionCriticality criticality, isl::string_view message,
            isl::string_view suggestion = {}) -> void;

        auto yield() CCL_LIFETIMEBOUND -> const Token &;
        auto yieldFutureToken() CCL_LIFETIMEBOUND -> const Token &;

    private:
        auto nextToken(Token &token) -> void;
        auto returnIfNotInIgnored(Token &token) -> void;

        [[nodiscard]] auto shouldIgnoreToken(const Token &token) const -> bool;

        auto constructBadToken(Token &token) -> void;
        auto constructEoiToken(Token &token) const -> void;
    };
}// namespace ccl::lexer

#endif /* CCL_PROJECT_TOKENIZER_HPP */

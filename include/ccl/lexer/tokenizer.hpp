#ifndef CCL_PROJECT_TOKENIZER_HPP
#define CCL_PROJECT_TOKENIZER_HPP

#include <ccl/lexer/lexical_analyzer.hpp>

namespace ccl::lexer
{
    class LexicalAnalyzer::Tokenizer
    {
        std::array<Token, 2> tokens;
        TextIterator textIterator;
        text::InputInfo inputInfo;
        const LexicalAnalyzer &lexicalAnalyzer;
        bool hasFutureToken{};
        bool tokenIndex{};

    public:
        [[nodiscard]] Tokenizer(
            const LexicalAnalyzer &lexical_analyzer, isl::string_view text,
            isl::string_view filename = {});

        [[nodiscard]] Tokenizer(
            const LexicalAnalyzer &lexical_analyzer, isl::string_view text,
            isl::string_view filename, const ExceptionHandler &exception_handler);

        [[nodiscard]] auto getIterator() const CCL_LIFETIMEBOUND -> const TextIterator &
        {
            return textIterator;
        }

        [[nodiscard]] auto getHandler() const CCL_LIFETIMEBOUND -> const ExceptionHandler &
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

        [[nodiscard]] auto
            getInputInfo() const noexcept CCL_LIFETIMEBOUND -> const text::InputInfo *
        {
            return std::addressof(inputInfo);
        }

        auto throwExceptionToHandler(
            ExceptionCriticality criticality, isl::string_view message,
            isl::string_view suggestion = {}) const -> void;

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

module;
#include <ccl/defines.hpp>
export module ccl.lexer:tokenizer;

export import isl;
export import ccl.text;
export import ccl.handler;
export import ccl.lexer.dot_item;
export import :lexical_analyzer;

export namespace ccl::lexer
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
            LexicalAnalyzer &lexical_analyzer, isl::string_view text,
            std::string_view filename = {});

        [[nodiscard]] Tokenizer(
            LexicalAnalyzer &lexical_analyzer, isl::string_view text, std::string_view filename,
            handler::ExceptionHandler &exception_handler);

        [[nodiscard]] auto getIterator() const CCL_LIFETIMEBOUND -> const TextIterator &
        {
            return textIterator;
        }

        [[nodiscard]] auto getHandler() CCL_LIFETIMEBOUND -> handler::ExceptionHandler &
        {
            return textIterator.getHandler();
        }

        [[nodiscard]] auto getCurrentToken() CCL_LIFETIMEBOUND -> Token &
        {
            return tokens[tokenIndex];// NOLINT
        }

        auto throwExceptionToHandler(
            text::ExceptionCriticality criticality, isl::string_view message,
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

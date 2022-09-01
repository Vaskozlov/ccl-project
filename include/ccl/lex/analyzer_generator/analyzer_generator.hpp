#ifndef CCL_PROJECT_ANALYZER_GENERATOR_HPP
#define CCL_PROJECT_ANALYZER_GENERATOR_HPP

#include <ccl/lex/lexical_analyzer.hpp>
#include <ccl/string_view.hpp>
#include <filesystem>
#include <string>

namespace ccl::lex
{
    // NOLINTNEXTLINE
    CCL_ENUM(
        GenToken,
        size_t,
        EOI = ReservedTokenType::EOI,
        BAD_TOKEN = ReservedTokenType::BAD_TOKEN,
        IDENTIFIER = 2,
        INTEGER = 3,
        NEW_LINE = 4,
        COLUMN = 5,
        ASSIGN = 6,
        CURLY_OPENING = 7,
        CURLY_CLOSING = 8,
        SEQUENCE = 9,
        UNION = 10,
        COMMENT = 11,
        STRING = 12,
        MULTILE_STRING = 13,
        CHARACTER = 14,
        RULE_DECLARATION = 100);

    // NOLINTNEXTLINE
    const inline std::map<GenToken, string_view> GenTokenNames{
        { GenToken::EOI, "EOI" },
        { GenToken::BAD_TOKEN, "BAD TOKEN" },
        { GenToken::RULE_DECLARATION, "rule declaration" },
        { GenToken::IDENTIFIER, "identifier" },
        { GenToken::INTEGER, "integer" },
        { GenToken::NEW_LINE, "new line" },
        { GenToken::COLUMN, "column" },
        { GenToken::ASSIGN, "assign" },
        { GenToken::CURLY_OPENING, "curly opening" },
        { GenToken::CURLY_CLOSING, "curly closing" },
        { GenToken::SEQUENCE, "sequence" },
        { GenToken::UNION, "union" },
    };

    class AnalyzerGenerator
    {
    public:
        static auto generateStaticVersion(const std::filesystem::path &path) -> void;
        static auto generateStaticVersion(string_view filename, string_view text) -> void;

    private:
        static LexicalAnalyzer LexForGenerator;// NOLINT non const variable
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_ANALYZER_GENERATOR_HPP */

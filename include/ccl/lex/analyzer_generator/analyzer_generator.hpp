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
        ANGLE_OPENING = 7,
        ANGLE_CLOSING = 8,
        RAW_DATA = 100);

    // NOLINTNEXTLINE
    const inline std::map<GenToken, u8string_view> GenTokenNames{
        { GenToken::EOI, u8"EOI" },
        { GenToken::BAD_TOKEN, u8"BAD TOKEN" },
        { GenToken::RAW_DATA, u8"RAW DATA" },
        { GenToken::IDENTIFIER, u8"IDENTIFIER" },
        { GenToken::INTEGER, u8"INTEGER" },
        { GenToken::NEW_LINE, u8"NEW LINE" },
        { GenToken::COLUMN, u8"COLUMN" },
        { GenToken::ASSIGN, u8"ASSIGN" },
        { GenToken::ANGLE_OPENING, u8"ANGLE OPENING" },
        { GenToken::ANGLE_CLOSING, u8"ANGLE CLOSING" }
    };

    CCL_EXCEPTION(RuleParsingException, text::TextIteratorException);

    class AnalyzerGenerator
    {
    public:
        static auto generateStaticVersion(const std::filesystem::path &path) -> void;
        static auto generateStaticVersion(u8string_view text, u8string_view filename) -> void;

    private:
        static LexicalAnalyzer LexForGenerator;// NOLINT non const variable
    };
}// namespace ccl::lex

#endif /* CCL_PROJECT_ANALYZER_GENERATOR_HPP */
